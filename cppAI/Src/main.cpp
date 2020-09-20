#pragma once

#include "Factory\Factory.h"

#include <iostream>

class Base {
public:
	Base() = default;
	// Destructor of base class must always be virtual
	virtual ~Base() = default;
	virtual auto getType() const -> std::string {
		return "Base";
	}
	void showType() {
		std::cout << "Class type = " << this->getType() << "\n";
	}
};
REGISTER_CLASS_TO_FACTORY2(Base, Base);

class DerivedA : public Base {
public:
	DerivedA() {}
	auto getType() const  -> std::string {
		return "DerivedA";
	}
};
REGISTER_CLASS_TO_FACTORY2(DerivedA, Base);

class DerivedB : public Base {
public:
	DerivedB() {}
	auto getType() const -> std::string {
		return "DerivedB";
	}
};
REGISTER_CLASS_TO_FACTORY2(DerivedB, Base);

class NonDerived {
public:
	NonDerived() {}
	void printMessage() {
		std::puts(" ==> I am a non derived class");
	}
};
REGISTER_CLASS_TO_FACTORY1(NonDerived);

int main() {
	Factory::showClasses();
	std::cout << "------- Test 1 ------------" << "\n";
	auto base = Factory::make<Base>("Base");
	base->showType();
	auto da = Factory::make<Base>("DerivedA");
	da->showType();
	auto db = Factory::make<Base>("DerivedB");
	db->showType();

	std::cout << "------- Test 2 ------------" << "\n";

	for (const auto& cls : { "Base", "DerivedA", "DerivedB" }) {
		auto obj = Factory::makeSafe<Base>(cls);
		std::cout << "Class [" << cls << " ] = " << obj->getType() << " \n";
	}

	std::cout << "------- Test 3 ------------" << "\n";
	std::unique_ptr<NonDerived> ndc = Factory::make<NonDerived>("NonDerived");
	ndc->printMessage();

	std::cout << "------- Test 4 ------------" << "\n";
	// Simulate exception
	try {
		std::unique_ptr<Base> dummy = Factory::makeSafe<Base>("NonDerived");
		dummy->showType();
	}
	catch (const runtime_error_location & ex) {
		std::cerr << " [ERROR] " << ex.what() << "\n";
	}

	return 0;
}