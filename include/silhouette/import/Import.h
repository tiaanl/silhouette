#ifndef SILHOUETTE_IMPORT_IMPORT_H_
#define SILHOUETTE_IMPORT_IMPORT_H_

#include "nucleus/FilePath.h"
#include "nucleus/Streams/InputStream.h"
#include "silhouette/scene/Scene.h"

namespace si {

bool loadCollada(Scene* scene, nu::InputStream* stream);
bool loadCollada(Scene* scene, const nu::FilePath& path);

}  // namespace si

#endif  // SILHOUETTE_IMPORT_IMPORT_H_
