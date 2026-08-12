#ifndef PRODUCTSERVICE_H
#define PRODUCTSERVICE_H

#include "../repository/ProductRepository.h"

class ProductService {
private:
    ProductRepository repository;

public:
    void addProduct(Product product) {
        repository.addProduct(product);
    }

    vector<Product> getAllProducts() {
        return repository.getAllProducts();
    }

    Product* getProductById(int id) {
        return repository.getProductById(id);
    }

    bool deleteProduct(int id) {
        return repository.deleteProduct(id);
    }
};

#endif