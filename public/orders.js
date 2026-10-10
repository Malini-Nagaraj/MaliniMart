const userId = 4;

// LOAD ORDER HISTORY
async function loadOrders() {

    const ordersList = document.getElementById("ordersList");

    try {

        const response = await fetch(`/orders?userId=${userId}`);

        const orders = await response.json();

        ordersList.innerHTML = "";

        if (!orders || orders.length === 0) {

            ordersList.innerHTML = `
                <p>You have no orders yet.</p>
            `;

            return;
        }

        orders.forEach(order => {

            const card = document.createElement("div");

            card.className = "product-card";

            card.innerHTML = `
                <h3>Order #${order.id}</h3>

                <p>
                    <strong>Total Amount:</strong>
                    ₹${order.totalAmount}
                </p>

                <p>
                    <strong>Status:</strong>
                    ${order.status}
                </p>

                <p>
                    <strong>Date:</strong>
                    ${order.createdAt}
                </p>
            `;

            ordersList.appendChild(card);

        });

    } catch (error) {

        console.error(error);

        ordersList.innerHTML = `
            <p>Unable to load orders.</p>
        `;
    }
}


// GO TO PRODUCTS
function goToProducts() {

    window.location.href = "products.html";
}


// LOAD ORDERS
loadOrders();