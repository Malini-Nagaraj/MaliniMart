const userId = 4;


async function loadCart() {

    const cartList =
        document.getElementById("cartList");

    const cartTotal =
        document.getElementById("cartTotal");

    try {

        const response =
            await fetch(`/cart?userId=${userId}`);

        const cart =
            await response.json();

        cartList.innerHTML = "";

        if (!cart.items || cart.items.length === 0) {

            cartList.innerHTML =
                "<p>Your cart is empty.</p>";

            cartTotal.innerHTML = "";

            return;
        }

        cart.items.forEach(item => {

            const card =
                document.createElement("div");

            card.className = "product-card";

            card.innerHTML = `
                <h3>${item.name}</h3>

                <p>Price: ₹${item.price}</p>

                <p>Quantity: ${item.quantity}</p>

                <p>Total: ₹${item.total}</p>

                <button onclick="removeItem(${item.id})">
                    Remove
                </button>
            `;

            cartList.appendChild(card);
        });

        cartTotal.innerHTML = `
            <h2>Grand Total: ₹${cart.grandTotal}</h2>
        `;

    } catch (error) {

        cartList.innerHTML =
            "Unable to load cart.";

        console.error(error);
    }
}


async function removeItem(cartId) {

    try {

        const response =
            await fetch(`/cart/remove?cartId=${cartId}`, {

                method: "DELETE"

            });

        const result =
            await response.json();

        if (response.ok) {

            alert("Product removed from cart.");

            loadCart();

        } else {

            alert(result.message ||
                  "Failed to remove product.");
        }

    } catch (error) {

        alert("Unable to connect to server.");

        console.error(error);
    }
}


function goToProducts() {

    window.location.href = "products.html";
}


function checkout() {

    window.location.href = "checkout.html";
}


loadCart();
