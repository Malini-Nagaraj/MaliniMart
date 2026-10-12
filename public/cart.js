
const userId = 4;

async function loadCart() {
    const cartList = document.getElementById("cartList");
    const cartTotal = document.getElementById("cartTotal");
    const itemCount = document.getElementById("itemCount");

    cartList.innerHTML = "<p>Loading your cart...</p>";

    try {
        const response = await fetch(`/cart?userId=${userId}`);

        if (!response.ok) {
            throw new Error("Failed to load cart");
        }

        const cart = await response.json();

        cartList.innerHTML = "";

        const items = Array.isArray(cart.items) ? cart.items : [];

        if (items.length === 0) {
            cartList.innerHTML = `
                <div class="empty-cart">
                    <h3>Your cart is empty 🛒</h3>
                    <p>Add a product to see it here.</p>
                    <button class="checkout-button"
                        onclick="goToProducts()">
                        Continue Shopping
                    </button>
                </div>
            `;

            cartTotal.textContent = "₹0.00";
            itemCount.textContent = "0";
            return;
        }

        let count = 0;

        items.forEach(item => {
            const card = document.createElement("article");
            card.className = "cart-item";

            const image = document.createElement("img");
            image.src = item.image ||
                "https://placehold.co/200x200?text=Product";
            image.alt = item.name || "Product";
            image.onerror = function () {
                this.onerror = null;
                this.src = "https://placehold.co/200x200?text=Product";
            };

            const info = document.createElement("div");
            info.className = "item-info";

            const name = document.createElement("h3");
            name.textContent = item.name || "Product";

            const price = document.createElement("p");
            price.className = "item-price";
            price.textContent = `₹${Number(item.price || 0).toLocaleString("en-IN")}`;

            const quantity = document.createElement("p");
            quantity.textContent = `Quantity: ${item.quantity}`;

            const total = document.createElement("p");
            total.textContent =
                `Subtotal: ₹${Number(item.total || 0).toLocaleString("en-IN")}`;

            const removeButton = document.createElement("button");
            removeButton.className = "remove-button";
            removeButton.textContent = "Remove";
            removeButton.onclick = () => removeItem(item.id);

            info.append(name, price, quantity, total, removeButton);
            card.append(image, info);
            cartList.appendChild(card);

            count += Number(item.quantity || 0);
        });

        itemCount.textContent = String(count);

        const grandTotal = Number(cart.grandTotal || 0);
        cartTotal.textContent = `₹${grandTotal.toLocaleString("en-IN", {
            minimumFractionDigits: 2,
            maximumFractionDigits: 2
        })}`;

    } catch (error) {
        console.error("Error loading cart:", error);

        cartList.innerHTML = `
            <div class="empty-cart">
                <h3>Unable to load your cart</h3>
                <p>Check your server connection and try again.</p>
                <button class="checkout-button" onclick="loadCart()">
                    Try Again
                </button>
            </div>
        `;

        cartTotal.textContent = "₹0.00";
        itemCount.textContent = "0";
    }
}

async function removeItem(cartId) {
    if (!confirm("Remove this product from your cart?")) {
        return;
    }

    try {
        const response = await fetch(
            `/cart/remove?cartId=${encodeURIComponent(cartId)}`,
            { method: "DELETE" }
        );

        if (!response.ok) {
            throw new Error("Failed to remove product");
        }

        alert("Product removed from cart.");
        await loadCart();

    } catch (error) {
        console.error(error);
        alert("Unable to remove the product. Please try again.");
    }
}

function goToProducts() {
    window.location.href = "products.html";
}

function checkout() {
    window.location.href = "checkout.html";
}

loadCart();
