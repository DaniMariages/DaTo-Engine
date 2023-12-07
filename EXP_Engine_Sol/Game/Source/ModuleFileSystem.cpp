#include "ModuleFileSystem.h"

ModuleFileSystem::ModuleFileSystem(Application* app, bool start_enabled) : Module(app, start_enabled) {}

ModuleFileSystem::~ModuleFileSystem() {}

bool ModuleFileSystem::Awake() { return true; }

bool ModuleFileSystem::Start() { return true; }

bool ModuleFileSystem::CleanUp() { return true; }

update_status ModuleFileSystem::Update(float dt) { return UPDATE_CONTINUE; }

