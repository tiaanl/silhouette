#include "silhouette/codec/scene/obj.h"

#include <fstream>
#include <string>

#include "../../ext/tiny_obj_loader.h"

namespace si {

namespace {

struct Index {
  MemSize position;
  MemSize normal;
  MemSize tex_coord;
};

struct Context {
  Scene* scene;
  Mesh* current_mesh = nullptr;

  nu::DynamicArray<fl::Vec3> positions;
  nu::DynamicArray<fl::Vec3> normals;
  nu::DynamicArray<fl::Vec2> tex_coords;

  nu::DynamicArray<Index> indices;

  explicit Context(Scene* scene) : scene{scene} {}
};

inline Context* to_context(void* user_data) {
  DCHECK(user_data);
  return reinterpret_cast<Context*>(user_data);
}

void finish_mesh(Context* ctx) {
  for (auto& index : ctx->indices) {
    ctx->current_mesh->positions.emplaceBack(ctx->positions[index.position]);
    ctx->current_mesh->normals.emplaceBack(ctx->normals[index.normal]);
    ctx->current_mesh->tex_coords.emplaceBack(ctx->tex_coords[index.tex_coord]);
  }

  ctx->current_mesh->material_index = 0;
  ctx->current_mesh = nullptr;
}

void do_object(void* user_data, const char* name) {
  auto ctx = to_context(user_data);

  if (ctx->current_mesh != nullptr) {
    finish_mesh(ctx);
  }

  DCHECK(ctx->current_mesh == nullptr);

  LOG(Info) << "name: " << name;

  auto result = ctx->scene->meshes().emplaceBack(name);
  ctx->current_mesh = &result.element();

  ctx->positions.clear();
  ctx->normals.clear();
  ctx->tex_coords.clear();
}

void do_vertex(void* user_data, tinyobj::real_t x, tinyobj::real_t y, tinyobj::real_t z,
               tinyobj::real_t) {
  auto ctx = to_context(user_data);
  DCHECK(ctx->current_mesh);

  ctx->positions.emplaceBack(x, y, z);
}

void do_normal(void* user_data, tinyobj::real_t x, tinyobj::real_t y, tinyobj::real_t z) {
  auto ctx = to_context(user_data);
  DCHECK(ctx->current_mesh);

  ctx->normals.emplaceBack(x, y, z);
}

void do_tex_coord(void* user_data, tinyobj::real_t x, tinyobj::real_t y, tinyobj::real_t z) {
  auto ctx = to_context(user_data);
  DCHECK(ctx->current_mesh);

  ctx->tex_coords.emplaceBack(x, y);
}

void do_index(void* user_data, tinyobj::index_t* indices, int num_indices) {
  auto ctx = to_context(user_data);
  DCHECK(ctx->current_mesh);

  for (int i = 0; i < num_indices; ++i) {
    ctx->indices.emplaceBack(static_cast<MemSize>(-1) + indices[i].vertex_index,
                             static_cast<MemSize>(-1) + indices[i].normal_index,
                             static_cast<MemSize>(-1) + indices[i].texcoord_index);
  }
}

}  // namespace

nu::Optional<Scene> load_scene_from_obj(nu::InputStream* stream) {
  std::string buffer;
  buffer.resize(stream->getBytesRemaining());
  auto bytes_read = stream->read(buffer.data(), buffer.size());
  if (bytes_read != buffer.size()) {
    LOG(Warning) << "Could not read from .obj file.";
    return {};
  }

  tinyobj::callback_t callbacks;
  callbacks.object_cb = do_object;
  callbacks.vertex_cb = do_vertex;
  callbacks.normal_cb = do_normal;
  callbacks.texcoord_cb = do_tex_coord;
  callbacks.index_cb = do_index;

  std::stringbuf buf{buffer};
  std::istream in_stream(&buf);

  Scene result;
  RGBA color{255, 0, 0, 255};
  result.materials().emplaceBack(color);

  Context context{&result};

  if (!tinyobj::LoadObjWithCallback(in_stream, callbacks, &context)) {
    LOG(Error) << "Could not load .obj file.";
    return {};
  }

  finish_mesh(&context);

  result.root_node().meshIndices.emplaceBack(static_cast<MemSize>(0));

  return result;
}

}  // namespace si
