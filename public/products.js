const userId = 4;

// LOAD PRODUCTS
async function loadProducts() {
    const productsList = document.getElementById("productsList");

    try {
        const response = await fetch("/products");
        const products = await response.json();

        productsList.innerHTML = "";

        products.forEach(product => {

            const card = document.createElement("div");
            card.className = "product-card";

            card.innerHTML = `
                <img
                    src="${product.image}"
                    alt="${product.name}"
                    class="product-image"
                    onerror="this.style.display='none'"
                >

                <h3>${product.name}</h3>

                <p>${product.description}</p>

                <p class="product-price">
                    ₹${product.price}
                </p>

                <button onclick="addToCart(${product.id})">
                    Add to Cart
                </button>
            `;

            productsList.appendChild(card);
        });

    } catch (error) {

        productsList.innerHTML =
            "Unable to load products.";

        console.error(error);
    }
}


// ADD PRODUCT TO CART
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

        const result = await response.json();

        if (response.ok) {

            alert("Product added to cart successfully!");

        } else {

            alert(result.message || "Failed to add product.");
        }

    } catch (error) {

        alert("Unable to connect to server.");

        console.error(error);
    }
}


// GO TO CART
function goToCart() {
    window.location.href = "cart.html";
}


// LOGOUT
function logout() {
    window.location.href = "index.html";
}


// LOAD PRODUCTS WHEN PAGE OPENS
loadProducts();