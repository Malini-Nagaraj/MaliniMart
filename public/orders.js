
const userId = 4;

// LOAD ORDER HISTORY
async function loadOrders() {
    const ordersList = document.getElementById("ordersList");

    ordersList.innerHTML = "<p>Loading your orders...</p>";

    try {
        const response = await fetch(`/orders?userId=${userId}`);

        if (!response.ok) {
            throw new Error("Failed to load orders");
        }

        const orders = await response.json();

        ordersList.innerHTML = "";

        if (!Array.isArray(orders) || orders.length === 0) {
            ordersList.innerHTML = `
                <div class="empty-orders">
                    <h3>No orders yet</h3>
                    <p>Your purchases will appear here.</p>
                    <button class="shopping-button"
                        onclick="goToProducts()">
                        Start Shopping
                    </button>
                </div>
            `;
            return;
        }

        orders.forEach(order => {
            const card = document.createElement("article");
            card.className = "order-card";

            const header = document.createElement("div");
            header.className = "order-card-header";

            const orderInfo = document.createElement("div");

            const orderTitle = document.createElement("h3");
            orderTitle.className = "order-id";
            orderTitle.textContent = `Order #${order.id}`;

            const date = document.createElement("div");
            date.className = "order-date";

            const rawDate = order.createdAt ?? order.created_at;

            if (rawDate) {
                const parsedDate = new Date(rawDate);
                date.textContent = Number.isNaN(parsedDate.getTime())
                    ? `Date: ${rawDate}`
                    : parsedDate.toLocaleString("en-IN", {
                        day: "2-digit",
                        month: "short",
                        year: "numeric",
                        hour: "2-digit",
                        minute: "2-digit"
                    });
            } else {
                date.textContent = "Date unavailable";
            }

            orderInfo.appendChild(orderTitle);
            orderInfo.appendChild(date);

            const status = document.createElement("span");
            status.className = "order-status";
            status.textContent = order.status || "Pending";

            header.appendChild(orderInfo);
            header.appendChild(status);

            const details = document.createElement("div");
            details.className = "order-details";

            const totalBox = document.createElement("div");

            const totalLabel = document.createElement("div");
            totalLabel.className = "order-total-label";
            totalLabel.textContent = "Total Amount";

            const total = document.createElement("p");
            total.className = "order-total";

            const amount = Number(order.totalAmount ?? order.total_amount);
            total.textContent = Number.isFinite(amount)
                ? `₹${amount.toLocaleString("en-IN", {
                    minimumFractionDigits: 2,
                    maximumFractionDigits: 2
                })}`
                : "Amount unavailable";

            totalBox.appendChild(totalLabel);
            totalBox.appendChild(total);

            const note = document.createElement("p");
            note.className = "order-note";
            note.textContent = "Thank you for shopping with MaliniMart!";

            details.appendChild(totalBox);
            details.appendChild(note);

            card.appendChild(header);
            card.appendChild(details);

            ordersList.appendChild(card);
        });

    } catch (error) {
        console.error("Error loading orders:", error);

        ordersList.innerHTML = `
            <div class="empty-orders">
                <h3>Unable to load orders</h3>
                <p>Please check that the MaliniMart server is running
                   and try refreshing the page.</p>
                <button class="shopping-button"
                    onclick="loadOrders()">
                    Try Again
                </button>
            </div>
        `;
    }
}

// GO TO PRODUCTS
function goToProducts() {
    window.location.href = "products.html";
}

// LOAD ORDERS
loadOrders();
