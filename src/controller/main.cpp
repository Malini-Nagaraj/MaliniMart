#include <iostream>
#include "controller/ProductController.h"

using namespace std;

int main() {

    ProductController controller;

    // Create products
    Product p1;
    p1.id = 1;
    p1.name = "Apple";
    p1.price = 120;

    Product p2;
    p2.id = 2;
    p2.name = "Milk";
    p2.price = 50;

    // Add products
    controller.addProduct(p1);
    controller.addProduct(p2);

    // Display all products
    cout << "===== MALINIMART PRODUCTS =====" << endl;
    controller.showAllProducts();

    // Find product
    cout << "\n===== SEARCH PRODUCT =====" << endl;
    controller.showProductById(1);

    // Delete product
    cout << "\n===== DELETE PRODUCT =====" << endl;
    controller.deleteProduct(2);

    // Display products again
    cout << "\n===== PRODUCTS AFTER DELETE =====" << endl;
    controller.showAllProducts();

    return 0;
}