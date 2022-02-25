#pragma once
#include <memory>

class Transform;
class ModelObject;
class DirectionLightObject;
class CameraObject;

typedef std::shared_ptr<Transform> TransformPtr;
typedef std::shared_ptr<ModelObject> ModelObjectPtr;
typedef std::shared_ptr<DirectionLightObject> DirectionLightObjectPtr;
typedef std::shared_ptr<CameraObject> CameraObjectPtr;