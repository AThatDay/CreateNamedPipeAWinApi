//// Class with deleted constructor
//class MyClass {
//public:
//    // Deleted default constructor
//    MyClass() = delete;
//
//    // Deleted copy constructor
//    MyClass(const MyClass&) = delete;
//
//    // Deleted move constructor
//    MyClass(MyClass&&) = delete;
//
//    // Other member functions can be declared normally
//    void someFunction() {
//        // Function implementation
//    }
//};
//int main() {
//    // Attempting to create an instance of MyClass will cause a compile-time error
//    // MyClass obj; // This line would cause a compilation error
//
//    // You can create pointers to MyClass
//    MyClass* ptr = nullptr;
//
//    // You can call member functions that are not deleted
//    ptr->someFunction();
//
//    return 0;
//}
