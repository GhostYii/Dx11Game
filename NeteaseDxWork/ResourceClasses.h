#pragma once
#include <memory>

class Resource;
class ResourceManager;

class Texture;
class TextureManager;

typedef std::shared_ptr<Resource> ResourcePtr;
typedef std::shared_ptr<Texture> TexturePtr;