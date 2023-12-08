#include "GameObject.h"
#include "Component.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "ComponentMesh.h"
#include <vector>

GameObject::GameObject(std::string name, GameObject* parent)
{
	Name = name;
	Parent = parent;
	UID = randomInt.GenerateRandomInt();

	drawTexture = true;

	//Only Scene Game Object has nullptr as a parent, so dont add a transform to it
	if (parent != nullptr)
	{
		transform = new ComponentTransform(this);
		AddComponent(transform);
	}
}

GameObject::~GameObject() 
{
	transform = nullptr;

	if (Parent != nullptr) Parent->EraseChild(this);

	for (size_t i = 0; i < components.size(); ++i)
	{
		Component* component = components[i];
		if (component != nullptr)
		{
			delete components[i];
			components[i] = nullptr;
		}
	}

	for (size_t i = 0; i < children.size(); ++i)
	{
		delete children[i];
		children[i] = nullptr;
	}
}

void GameObject::Enable() //Start up + bool toggle
{
	this->active = true;
}

void GameObject::Disable()
{
	this->active = false;
}

void GameObject::EnableParent()
{
	this->active = true;
	for (unsigned int i = 0; i < this->children.size(); i++)
	{
		children[i]->EnableParent();
	}
}

void GameObject::DisableParent()
{
	this->active = false;
	for (unsigned int i = 0; i < this->children.size(); i++)
	{
		children[i]->DisableParent();
	}
}

void GameObject::EnableTexture()
{
	this->drawTexture = true;
}

void GameObject::DisableTexture()
{
	this->drawTexture = false;
}

void GameObject::EnableTextureParent()
{
	this->drawTexture = true;
	for (unsigned int i = 0; i < this->children.size(); i++)
	{
		children[i]->EnableTexture();
	}
}

void GameObject::DisableTextureParent()
{
	this->drawTexture = false;
	for (unsigned int i = 0; i < this->children.size(); i++)
	{
		children[i]->DisableTexture();
	}
}

void GameObject::ChangeName(const char* name) 
{
	Name = name;
}

void GameObject::Update() 
{
	if (!components.empty())
	{
		std::vector<Component*>::iterator item = components.begin();
		bool ret = true;

		for (; item != components.end() && ret == true; ++item)
		{
			(*item)->Update();
		}
	}
}

void GameObject::SetParent(GameObject* parent)
{
	Parent = parent;
}

Component* GameObject::AddComponent(Component* component)
{
	Component* ret = nullptr;

	switch (component->type)
	{
	case(typeComponent::Error):
		//LOG("Component Type Error! Something broke...");
		break;

	case(typeComponent::Transform):
		ret = new ComponentTransform(this);
		LOG("Component Transform added to %s", component->parent->Name.c_str());
		break;

	case(typeComponent::Material):
		if (this->GetComponent(typeComponent::Material) != nullptr) {
			return nullptr;
		}
		ret = new ComponentTexture(this);
		LOG("Component Texture added to %s", component->parent->Name.c_str());
		break;

	case(typeComponent::Mesh):
		ret = new ComponentMesh(this);
		LOG("Component mesh added to %s", component->parent->Name.c_str());
		break;

	case(typeComponent::Camera):
		ret = new ComponentCamera(this);
		LOG("Component Camera added to %s", component->parent->Name.c_str());
		break;
	}

	components.push_back(component);
	return ret;
}

GameObject* GameObject::AddChildren(GameObject* children) 
{
	if (this->Parent != nullptr)
	{
		GameObject* parentObject = this->Parent;

		while (parentObject != ExternalApp->scene->rootGameObject)
		{
			if (parentObject == children)
			{
				LOG("ERROR: Can't add %s to %s, they are already parented", children->Name.c_str(), this->Name.c_str());
				return nullptr;
			}
			else if (parentObject->Parent != nullptr)
			{
				parentObject = parentObject->Parent;
			}
		}
	}

	if (children->Parent != nullptr)
	{
		children->Parent->EraseChild(children); 
		children->SetParent(this);
	}
	else if (children->Parent == nullptr)
	{
		children->SetParent(this);
	}


	this->children.push_back(children);
	return children;
}

void GameObject::EraseChild(GameObject* child)
{
	if (!children.empty())
	{
		for (uint i = 0; i < children.size(); ++i)
		{
			if (children[i] == child)
			{
				children.erase(children.begin() + i);
			}
		}
	}
}

Component* GameObject::GetComponent(typeComponent type)
{
	std::vector<Component*>::iterator item = components.begin();

	for (; item != components.end(); ++item) 
	{
		if ((*item)->type == type) 
		{
			return (*item);
		}
	}
	return nullptr;
}

bool GameObject::HasComponent(typeComponent type)
{
	std::vector<Component*>::iterator item = components.begin();

	for (; item != components.end(); ++item)
	{
		if ((*item)->type == type)
		{
			return true;
		}
	}
	return false;
}

std::vector<Component*> GameObject::GetComponents(typeComponent type)
{
	std::vector<Component*>::iterator item = components.begin();

	std::vector<Component*> ret;

	for (; item != components.end(); ++item) 
	{
		if ((*item)->type == type) {
			ret.push_back((*item));
		}
	}

	return ret;
}

const uint GameObject::GetUID() const
{
	return UID;
}

void GameObject::SetUID(uint UID)
{
	this->UID = UID;
}

bool GameObject::IsRootObject()
{
	if (this == ExternalApp->scene->rootGameObject) return true;
	else return false;
}

bool GameObject::IsSelected()
{
	if (this == ExternalApp->scene->gameObjectSelected) return true;
	else return false;
}