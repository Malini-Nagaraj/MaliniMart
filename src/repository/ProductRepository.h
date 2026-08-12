#ifndef PRODUCTREPOSITORY_H
#define PRODUCTREPOSITORY_H

#include "../model/Product.h"
#include <vector>
#include <string>

using namespace std;

class ProductRepository {
private:
    vector<Product> products;

public:
    // Add a product
    void addProduct(Product product) {
        products.push_back(product);
    }

    // Get all products
    vector<Product> getAllProducts() {
        return products;
    }

    // Find product by ID
    Product* getProductById(int id) {
        for (auto& product : products) {
            if (product.id == id) {
                return &product;
            }
        }
        return nullptr;
    }

    // Delete product by ID
    bool deleteProduct(int id) {
        for (auto it = products.begin(); it != products.end(); ++it) {
            if (it->id == id) {
                products.erase(it);
                return true;
            }
        }
        return false;
    }
};

#endif