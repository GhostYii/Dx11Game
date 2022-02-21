#pragma once
#include <memory>

class Resource;
class ResourceManager;

class Texture;
class TextureManager;

class Mesh;
class MeshManager;

typedef std::shared_ptr<Resource> ResourcePtr;
typedef std::shared_ptr<Texture> TexturePtr;
typedef std::shared_ptr<Mesh> MeshPtr;