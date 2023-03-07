#pragma once

#include <string>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/serialization.hpp>

class MyClass {
    private:
        std::string name;
        int age;
        float salary;

    public:
        MyClass(std::string name, int age, float salary) {
            this->name = name;
            this->age = age;
            this->salary = salary;
        }

        MyClass() {};

        template<class Archive>
        void serialize(Archive& ar, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(name);
            ar & BOOST_SERIALIZATION_NVP(age);
            ar & BOOST_SERIALIZATION_NVP(salary);
        }

        std::string getName() {
            return name;
        }

        int getAge() {
            return age;
        }

        float getSalary() {
            return salary;
        }

        void setName(std::string name) {
            this->name = name;
        }

        void setAge(int age) {
            this->age = age;
        }

        void setSalary(float salary) {
            this->salary = salary;
        }
};