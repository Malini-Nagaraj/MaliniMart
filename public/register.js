document
    .getElementById("registerForm")
    .addEventListener("submit", async function(event) {

        event.preventDefault();

        const name =
            document.getElementById("name").value;

        const email =
            document.getElementById("email").value;

        const password =
            document.getElementById("password").value;

        const message =
            document.getElementById("message");

        try {

            const response = await fetch("/users/register", {

                method: "POST",

                headers: {
                    "Content-Type": "application/json"
                },

                body: JSON.stringify({
                    name: name,
                    email: email,
                    password: password
                })

            });

            const result = await response.text();

            if (response.ok) {

                message.style.color = "green";

                message.textContent = result;

            } else {

                message.style.color = "red";

                message.textContent = result;
            }

        } catch (error) {

            message.style.color = "red";

            message.textContent =
                "Unable to connect to server.";

            console.error(error);
        }

    });