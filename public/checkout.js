const userId = 4;


// SHOW CHECKOUT MESSAGE
function loadCheckout() {

    const details = document.getElementById("checkoutDetails");

    details.innerHTML = `
        <p>Your cart is ready for checkout.</p>

        <p>
            Payment method:
            <strong>Mock Payment</strong>
        </p>

        <p>
            Click <strong>Confirm & Pay</strong>
            to place your order.
        </p>
    `;
}


// MAKE PAYMENT / PLACE ORDER
async function makePayment() {

    const resultDiv = document.getElementById("result");
    const paymentButton = document.getElementById("paymentButton");

    paymentButton.disabled = true;

    resultDiv.innerHTML = "Processing your order...";

    try {

        const response = await fetch("/checkout", {

            method: "POST",

            headers: {
                "Content-Type": "application/json"
            },

            body: JSON.stringify({
                userId: userId
            })
        });

        const result = await response.json();

        if (response.ok) {

            resultDiv.innerHTML = `
                <div class="product-card">

                    <h2>Order Confirmed! ✅</h2>

                    <p>
                        ${result.message}
                    </p>

                    <p>
                        <strong>Order ID:</strong>
                        ${result.orderId}
                    </p>

                    <p>
                        <strong>Status:</strong>
                        ${result.status}
                    </p>

                    <p>
                        <strong>Total Amount:</strong>
                        ₹${result.totalAmount}
                    </p>

                    <button onclick="goToProducts()">
                        Continue Shopping
                    </button>

                </div>
            `;

            paymentButton.style.display = "none";

        } else {

            resultDiv.innerHTML = `
                <p>
                    ${result.message || "Checkout failed."}
                </p>
            `;

            paymentButton.disabled = false;
        }

    } catch (error) {

        console.error(error);

        resultDiv.innerHTML =
            "<p>Unable to connect to server.</p>";

        paymentButton.disabled = false;
    }
}


// GO TO CART
function goToCart() {

    window.location.href = "cart.html";
}


// GO TO PRODUCTS
function goToProducts() {

    window.location.href = "products.html";
}


loadCheckout();