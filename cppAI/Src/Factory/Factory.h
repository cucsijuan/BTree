#pragma once

#include <vcruntime_exception.h>
#include <string>
#include <typeinfo>
#include <memory>
#include <map>

#define RUNTIME_ERROR_LOCATION(message) \
	runtime_error_location(__LINE__, __FILE__, message)

#define REGISTER_CLASS_TO_FACTORY1(aclass) \
	namespace { auto register_ ## derived = FactoryImpl<aclass, aclass>( #aclass ); }

#define REGISTER_CLASS_TO_FACTORY2(derivedClass, baseClass) \
	namespace { auto register_ ## derivedClass = FactoryImpl<derivedClass, baseClass>( #derivedClass ); }

struct runtime_error_location : public std::exception {
	std::string  message;
	runtime_error_location(const std::string& text)
		: message{ text }
	{
	}
	runtime_error_location(
		unsigned int line,
		const std::string& file,
		const std::string& text
	)
	{
		message = file + ":" + std::to_string(line) + ": " + "Error: " + text;
	}
	auto what() const noexcept -> const char* {
		return message.c_str();
	}
};

class Factory
{
public:
	virtual ~Factory() = default;

	virtual void* create() const = 0;

	virtual void destroy(void* object) const = 0;

	virtual size_t size() const = 0;
	
	virtual const std::type_info& typeinfo() const = 0;

	static void registerFactory(const std::string& name, Factory* factory);

	static void showClasses();

	template<class BaseClass>
	static std::unique_ptr<BaseClass> make(const std::string& name)
	{
		FactoryMap& reg = Factory::getRegister();
		auto it = reg.find(name);

		if (it == reg.end())
			return nullptr;

		if (it->second->typeinfo() != typeid(BaseClass))
			return nullptr;

		void* ptr = it->second->create();

		return std::unique_ptr<BaseClass>(reinterpret_cast<BaseClass*>(ptr));
	}

	template<class BaseClass>
	static std::unique_ptr<BaseClass> makeSafe(const std::string& name)
	{
		auto object = Factory::make<BaseClass>(name);

		if (object == nullptr)
			throw RUNTIME_ERROR_LOCATION(std::string("Cannot create type. Failed to cast void* to: ") + name);

		return object;
	}

private:
	using FactoryMap = std::map<std::string, Factory*>;

	static FactoryMap& getRegister()
	{
		static FactoryMap classRegister{};
		return classRegister;
	}
};

template<typename DerivedClass, typename BaseClass = DerivedClass>
class FactoryImpl : Factory {
private:
	const std::type_info& _tinfo;
public:
	// Register this global object on the Factory register
	FactoryImpl(const std::string& name)
		: _tinfo(typeid(BaseClass))
	{
		std::cerr << " [TRACE] " << " Registered Class = " << name << "\n";
		Factory::registerFactory(name, this);
	}
	// Capture class default constructor => constructor type erasure 
	void* create() const override {
		return new DerivedClass;
	}

	void destroy(void* object) const override
	{
		delete reinterpret_cast<BaseClass*>(object);
	}

	const std::type_info& typeinfo() const override {
		return _tinfo;
	}
	size_t size() const override {
		return sizeof(DerivedClass);
	}

};


