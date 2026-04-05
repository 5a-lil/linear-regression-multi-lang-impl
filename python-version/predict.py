import json
import os

THETA_FILE = "thetas.json"

def load_thetas():
    if not os.path.exists(THETA_FILE):
        return 0.0, 0.0
    with open(THETA_FILE, "r") as f:
        data = json.load(f)
    return data["theta0"], data["theta1"]

def estimate_price(mileage, theta0, theta1):
    return theta0 + (theta1 * mileage)

def main():
    theta0, theta1 = load_thetas()

    try:
        mileage = float(input("Enter mileage: "))
    except ValueError:
        print("Invalid input. Please enter a numeric value.")
        return

    price = estimate_price(mileage, theta0, theta1)
    print(f"Estimated price: {price:.2f}")

if __name__ == "__main__":
    main()
