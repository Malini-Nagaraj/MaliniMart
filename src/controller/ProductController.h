#ifndef PRODUCTCONTROLLER_H
#define PRODUCTCONTROLLER_H

#include "../service/ProductService.h"
#include <iostream>

using namespace std;

class ProductController {
private:
    ProductService service;

public:
    void addProduct(Product product) {
        service.addProduct(product);
    }

    void showAllProducts() {
        vector<Product> products = service.getAllProducts();

        for (const auto& product : products) {
            cout << "ID: " << product.id << endl;
            cout << "Name: " << product.name << endl;
            cout << "Price: " << product.price << endl;
            cout << "----------------------" << endl;
        }
    }

    void showProductById(int id) {
        Product* product = service.getProductById(id);

        if (product != nullptr) {
            cout << "ID: " << product->id << endl;
            cout << "Name: " << product->name << endl;
            cout << "Price: " << product->price << endl;
        } else {
            cout << "Product not found!" << endl;
        }
    }

    void deleteProduct(int id) {
        if (service.deleteProduct(id)) {
            cout << "Product deleted successfully!" << endl;
        } else {
            cout << "Product not found!" << endl;
        }
    }
};

#endif