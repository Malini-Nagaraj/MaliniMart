const userId = 4;

async function loadProducts() {
    const productsList = document.getElementById("productsList");

    productsList.innerHTML = "<p>Loading products...</p>";

    try {
        const response = await fetch("/products");

        if (!response.ok) {
            throw new Error("Failed to load products");
        }

        const products = await response.json();

        console.log("Products received:", products);

        productsList.innerHTML = "";

        if (!Array.isArray(products) || products.length === 0) {
            productsList.innerHTML = "<h2>No products found.</h2>";
            return;
        }

        products.forEach(function(product) {
            const card = document.createElement("div");
            card.className = "product-card";

            // Product image
            const image = document.createElement("img");
            image.className = "product-image";
            image.alt = product.name || "Product image";
            image.src = product.image || "https://placehold.co/400x300?text=Product";

            image.onerror = function() {
                image.onerror = null;
                image.src = "https://placehold.co/400x300?text=Image+Unavailable";
            };

            // Product name
            const name = document.createElement("h3");
            name.textContent = product.name;

            // Product description
            const description = document.createElement("p");
            description.textContent = product.description || "No description available.";

            // Product price
            const price = document.createElement("h3");
            price.textContent = "₹" + Number(product.price).toLocaleString("en-IN");

            // Add to Cart button
            const button = document.createElement("button");
            button.textContent = "Add to Cart";

            button.onclick = function() {
                addToCart(product.id);
            };

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
            "<h3>Could not load products. Please check the server.</h3>";
    }
}

async function addToCart(productId) {
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

        if (response.ok) {
            alert("Product added to cart!");
        } else {
            alert("Unable to add product to cart.");
        }
    } catch (error) {
        console.error(error);
        alert("Server connection failed.");
    }
}

function goToCart() {
    window.location.href = "cart.html";
}

function goToOrders() {
    window.location.href = "orders.html";
}

function logout() {
    window.location.href = "index.html";
}

loadProducts();