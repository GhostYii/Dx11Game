#pragma once
#include <memory>

class Resource;
class ResourceManager;

class Texture;

typedef std::shared_ptr<Resource> ResourcePtr;
typedef std::shared_ptr<Texture> TexturePtr;