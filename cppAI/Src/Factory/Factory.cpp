#include "Factory.h"
#include <iostream>

void Factory::registerFactory(const std::string& name, Factory* factory)
{
	auto& reg = Factory::getRegister();
	reg[name] = factory;
}

void Factory::showClasses()
{
	std::cout << " Registered classes. " << "\n";
	std::cout << " =================== " << "\n";
	for (const auto& pair : Factory::getRegister())
		std::cout << " + " << pair.first
		<< " ; RTTI name = "
		<< pair.second->typeinfo().name()
		<< " ; size (bytes) = "
		<< pair.second->size()
		<< "\n";
}

/*
template<class BaseClass>
static std::unique_ptr<BaseClass> Factory::make(const std::string& name)
{
	FactoryMap& reg = Factory::getRegister();
	auto it = reg.find(name);

	if (it == reg.end())
		return nullptr;

	if (it->second->typeinfo() != typeid(BaseClass))
		return nullptr;

	void* ptr = it->second->create();

	return std::unique_ptr<BaseClass>(reinterpret_cast<BaseClass*>(ptr));
}*/

/*
template<class BaseClass>
static std::unique_ptr<BaseClass> Factory::makeSafe(const std::string& name)
{
	auto object = Factory::make<BaseClass>(name);

	if (object == nullptr)
		throw RUNTIME_ERROR_LOCATION(std::string("Cannot create type. Failed to cast void* to: ") + name);

	return object;
}
*/
