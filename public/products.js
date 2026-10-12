
const userId = 4;

// Get a suitable image for each product
function getProductImage(product) {
    if (product.image && product.image.trim() !== "") {
        return product.image;
    }

    const name = (product.name || "").toLowerCase();

    if (name.includes("headphone")) {
        return "https://images.unsplash.com/photo-1505740420928-5e560c06d30e?w=600&auto=format&fit=crop";
    }

    if (name.includes("mouse")) {
        return "https://images.unsplash.com/photo-1527814050087-3793815479db?w=600&auto=format&fit=crop";
    }

    if (name.includes("speaker")) {
        return "https://images.unsplash.com/photo-1608043152269-423dbba4e7e1?w=600&auto=format&fit=crop";
    }

    return "https://placehold.co/600x400/f1f5f9/334155?text=Product";
}

// Load products from the C++ backend
async function loadProducts() {
    const productsList = document.getElementById("productsList");

    if (!productsList) {
        console.error("Element with ID productsList was not found.");
        return;
    }

    productsList.innerHTML = "<p>Loading products...</p>";

    try {
        const response = await fetch("/products");

        if (!response.ok) {
            throw new Error("Failed to load products: " + response.status);
        }

        const products = await response.json();

        console.log("Products received:", products);
        productsList.innerHTML = "";

        if (!Array.isArray(products) || products.length === 0) {
            productsList.innerHTML = "<h2>No products found.</h2>";
            return;
        }

        products.forEach(function (product) {
            const card = document.createElement("div");
            card.className = "product-card";

            const image = document.createElement("img");
            image.className = "product-image";
            image.src = getProductImage(product);
            image.alt = product.name || "Product image";
            image.loading = "lazy";

            image.onerror = function () {
                image.onerror = null;
                image.src =
                    "https://placehold.co/600x400/f1f5f9/334155?text=Image+Unavailable";
            };

            const name = document.createElement("h3");
            name.textContent = product.name || "Unnamed product";

            const description = document.createElement("p");
            description.textContent =
                product.description || "No description available.";

            const price = document.createElement("h3");
            const amount = Number(product.price);
            price.textContent = Number.isFinite(amount)
                ? "₹" + amount.toLocaleString("en-IN")
                : "Price unavailable";

            const button = document.createElement("button");
            button.textContent = "Add to Cart";
            button.type = "button";

            button.addEventListener("click", function () {
                addToCart(product.id, button);
            });

            card.appendChild(image);
            card.appendChild(name);
            card.appendChild(description);
            card.appendChild(price);
            card.appendChild(button);

            productsList.appendChild(card);
        });
    } catch (error) {
        console.error("Error loading products:", error);
        productsList.innerHTML =
            "<h3>Could not load products. Please try again later.</h3>";
    }
}

// Add a product to the cart
async function addToCart(productId, button) {
    const originalText = button.textContent;
    button.disabled = true;
    button.textContent = "Adding...";

    try {
        const response = await fetch("/cart/add", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({
                userId: userId,
                productId: productId,
                quantity: 1
            })
        });

        if (!response.ok) {
            throw new Error("Unable to add product to cart.");
        }

        alert("Product added to cart!");
    } catch (error) {
        console.error("Add to cart error:", error);
        alert("Unable to add the product. Please check the server and try again.");
    } finally {
        button.disabled = false;
        button.textContent = originalText;
    }
}

// Navigation functions
function goToCart() {
    window.location.href = "cart.html";
}

function goToOrders() {
    window.location.href = "orders.html";
}

function logout() {
    window.location.href = "index.html";
}

// Start loading products
document.addEventListener("DOMContentLoaded", loadProducts);
