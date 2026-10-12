
const userId = 4;

async function loadCheckout() {
    const itemsContainer = document.getElementById("checkoutItems");
    const totalElement = document.getElementById("checkoutTotal");

    try {
        const response = await fetch(`/cart?userId=${userId}`);

        if (!response.ok) {
            throw new Error("Could not load cart");
        }

        const cart = await response.json();

        itemsContainer.innerHTML = "";

        if (!cart.items || cart.items.length === 0) {
            itemsContainer.textContent = "Your cart is empty.";
            totalElement.textContent = "₹0.00";
            return;
        }

        cart.items.forEach(item => {
            const row = document.createElement("p");
            row.textContent =
                `${item.name} × ${item.quantity} — ₹${item.total}`;
            itemsContainer.appendChild(row);
        });

        totalElement.textContent =
            `₹${Number(cart.grandTotal).toFixed(2)}`;

    } catch (error) {
        itemsContainer.textContent =
            "Unable to load your cart. Please try again.";
        console.error(error);
    }
}

document.getElementById("checkoutForm").addEventListener(
    "submit",
    async function (event) {
        event.preventDefault();

        const message = document.getElementById("message");
        const button = this.querySelector('button[type="submit"]');

        button.disabled = true;
        message.textContent = "Placing your order...";

        try {
            const response = await fetch("/orders/checkout", {
                method: "POST",
                headers: {
                    "Content-Type": "application/json"
                },
                body: JSON.stringify({ userId: userId })
            });

            const result = await response.json().catch(() => ({}));

            if (!response.ok) {
                throw new Error(
                    result.message || "Checkout failed. Please try again."
                );
            }

            const confirmation = new URLSearchParams({
                orderId: result.orderId,
                totalAmount: result.totalAmount,
                status: result.status
            });

            window.location.href =
                `/confirmation.html?${confirmation.toString()}`;

        } catch (error) {
            message.textContent = error.message;
            button.disabled = false;
        }
    }
);

loadCheckout();
