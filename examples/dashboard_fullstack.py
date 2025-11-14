

import webviewtk as wv
import datetime
import random

# ============================================================================
# BACKEND LAYER - Pure Tauraro Logic
# ============================================================================

class AnalyticsBackend:
    """Backend service handling all data and business logic"""

    def __init__(self):
        self.users = []
        self.transactions = []
        self.products = []
        self.analytics_cache = {}
        self.initialize_data()

    def initialize_data(self):
        """Initialize sample data - In real app, this would connect to database"""
        print("[Backend] Initializing data...")

        # Generate users
        self.users = self.generate_users(150)

        # Generate products
        self.products = self.generate_products(25)

        # Generate transactions
        self.transactions = self.generate_transactions(500)

        # Calculate analytics
        self.refresh_analytics()

        print(f"[Backend] Initialized: {len(self.users)} users, {len(self.products)} products, {len(self.transactions)} transactions")

    def generate_users(self, count):
        """Generate sample user data"""
        users = []
        names = ["Alice", "Bob", "Charlie", "Diana", "Eve", "Frank", "Grace", "Henry", "Ivy", "Jack"]
        countries = ["USA", "UK", "Canada", "Australia", "Germany", "France", "Japan", "Brazil"]

        for i in range(count):
            user = {
                "id": i + 1,
                "name": f"{random.choice(names)} {chr(65 + random.randint(0, 25))}",
                "email": f"user{i+1}@example.com",
                "country": random.choice(countries),
                "joined": self.random_date("2023-01-01", "2024-12-31"),
                "active": random.random() > 0.2
            }
            users.append(user)

        return users

    def generate_products(self, count):
        """Generate sample product data"""
        products = []
        categories = ["Electronics", "Clothing", "Food", "Books", "Home", "Sports"]

        for i in range(count):
            product = {
                "id": i + 1,
                "name": f"Product {i+1}",
                "category": random.choice(categories),
                "price": round(random.uniform(10, 500), 2),
                "stock": random.randint(0, 1000),
                "rating": round(random.uniform(3.0, 5.0), 1)
            }
            products.append(product)

        return products

    def generate_transactions(self, count):
        """Generate sample transaction data"""
        transactions = []

        for i in range(count):
            user = random.choice(self.users)
            product = random.choice(self.products)
            quantity = random.randint(1, 5)

            transaction = {
                "id": i + 1,
                "user_id": user["id"],
                "product_id": product["id"],
                "quantity": quantity,
                "amount": round(product["price"] * quantity, 2),
                "date": self.random_date("2024-01-01", "2024-12-31"),
                "status": random.choice(["completed", "pending", "cancelled"])
            }
            transactions.append(transaction)

        return transactions

    def random_date(self, start, end):
        """Generate random date between start and end"""
        # Simple date generation (in real app would use datetime module)
        months = ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"]
        month = random.choice(months)
        day = random.randint(1, 28)
        year = 2024
        return f"{month} {day}, {year}"

    def refresh_analytics(self):
        """Calculate analytics from data"""
        print("[Backend] Calculating analytics...")

        # Total revenue
        total_revenue = sum([t["amount"] for t in self.transactions if t["status"] == "completed"])

        # Active users
        active_users = len([u for u in self.users if u["active"]])

        # Total orders
        total_orders = len([t for t in self.transactions if t["status"] == "completed"])

        # Average order value
        avg_order_value = total_revenue / total_orders if total_orders > 0 else 0

        # Revenue by category
        revenue_by_category = {}
        for trans in self.transactions:
            if trans["status"] == "completed":
                product = self.get_product(trans["product_id"])
                if product:
                    category = product["category"]
                    if category in revenue_by_category:
                        revenue_by_category[category] += trans["amount"]
                    else:
                        revenue_by_category[category] = trans["amount"]

        # Recent activity
        recent_transactions = sorted(self.transactions, key=lambda x: x["date"], reverse=True)[:10]
        recent_activity = []
        for trans in recent_transactions:
            user = self.get_user(trans["user_id"])
            product = self.get_product(trans["product_id"])
            if user and product:
                recent_activity.append({
                    "user": user["name"],
                    "action": f"Purchased {product['name']}",
                    "amount": trans["amount"],
                    "date": trans["date"]
                })

        self.analytics_cache = {
            "total_revenue": round(total_revenue, 2),
            "active_users": active_users,
            "total_orders": total_orders,
            "avg_order_value": round(avg_order_value, 2),
            "revenue_by_category": revenue_by_category,
            # Also store an items list as a fallback for VMs that don't iterate nested dicts correctly
            "revenue_by_category_items": list(revenue_by_category.items()),
            "recent_activity": recent_activity
        }

        print(f"[Backend] Analytics ready: Revenue ${total_revenue:.2f}, Users {active_users}, Orders {total_orders}")

    def get_user(self, user_id):
        """Get user by ID"""
        for user in self.users:
            if user["id"] == user_id:
                return user
        return None

    def get_product(self, product_id):
        """Get product by ID"""
        for product in self.products:
            if product["id"] == product_id:
                return product
        return None

    def get_analytics(self):
        """Get cached analytics data"""
        return self.analytics_cache

    def add_transaction(self, user_id, product_id, quantity):
        """Add new transaction - Business logic"""
        print(f"[Backend] Adding transaction: User {user_id}, Product {product_id}, Qty {quantity}")

        user = self.get_user(user_id)
        product = self.get_product(product_id)

        if not user:
            return {"success": False, "error": "User not found"}

        if not product:
            return {"success": False, "error": "Product not found"}

        if product["stock"] < quantity:
            return {"success": False, "error": "Insufficient stock"}

        # Create transaction
        transaction = {
            "id": len(self.transactions) + 1,
            "user_id": user_id,
            "product_id": product_id,
            "quantity": quantity,
            "amount": round(product["price"] * quantity, 2),
            "date": "Dec 31, 2024",
            "status": "completed"
        }

        self.transactions.append(transaction)

        # Update stock
        product["stock"] -= quantity

        # Refresh analytics
        self.refresh_analytics()

        print(f"[Backend] Transaction added successfully: ${transaction['amount']}")
        return {"success": True, "transaction": transaction}

    def get_top_products(self, limit=5):
        """Get top selling products"""
        product_sales = {}

        for trans in self.transactions:
            if trans["status"] == "completed":
                pid = trans["product_id"]
                if pid in product_sales:
                    product_sales[pid] += trans["quantity"]
                else:
                    product_sales[pid] = trans["quantity"]

        # Sort by sales
        sorted_products = sorted(product_sales.items(), key=lambda x: x[1], reverse=True)[:limit]

        top_products = []
        for product_id, sales in sorted_products:
            product = self.get_product(product_id)
            if product:
                top_products.append({
                    "name": product["name"],
                    "category": product["category"],
                    "sales": sales,
                    "revenue": round(product["price"] * sales, 2)
                })

        return top_products

    def get_user_statistics(self):
        """Get user statistics by country"""
        country_stats = {}

        for user in self.users:
            country = user["country"]
            if country in country_stats:
                country_stats[country]["total"] += 1
                if user["active"]:
                    country_stats[country]["active"] += 1
            else:
                country_stats[country] = {
                    "total": 1,
                    "active": 1 if user["active"] else 0
                }

        return country_stats


# ============================================================================
# FRONTEND LAYER - UI Generation
# ============================================================================

def create_dashboard_ui(backend):
    """Create the dashboard UI using analytics data from backend"""

    # backend.initialize_data()
    backend.refresh_analytics()
    analytics = backend.get_analytics()
    top_products = backend.get_top_products(5)
    user_stats = backend.get_user_statistics()

    # Safe local copies with defaults to avoid KeyError if analytics dict is missing keys
    total_revenue = analytics.get("total_revenue", 0)
    active_users = analytics.get("active_users", 0)
    total_orders = analytics.get("total_orders", 0)
    avg_order_value = analytics.get("avg_order_value", 0.0)
    recent_activity = analytics.get("recent_activity", [])

    # Note: For future IPC implementation, we could use Alpine.js for dynamic state
    # alpine_data = {
    #     "revenue": analytics["total_revenue"],
    #     "users": analytics["active_users"],
    #     "orders": analytics["total_orders"],
    #     "avgOrder": analytics["avg_order_value"],
    #     "recentActivity": analytics["recent_activity"],
    #     "topProducts": top_products
    # }

    # Category revenue chart data
    # Use a precomputed items list when available to avoid iterating nested dicts in some VM backends
    revenue_items = analytics.get("revenue_by_category_items")
    if revenue_items is None:
        if "revenue_by_category" in analytics:
            revenue_items = list(analytics["revenue_by_category"].items())
        else:
            revenue_items = []

    # Find max revenue manually
    max_revenue = 1
    for category, revenue in revenue_items:
        if revenue > max_revenue:
            max_revenue = revenue

    chart_bars = ""
    i = 0
    for category, revenue in revenue_items:
        height_percent = (revenue / max_revenue) * 100
        delay = i * 0.1
        chart_bars += wv.div(
            wv.render(
                wv.div(
                    wv.span(f"${int(revenue/1000)}K", "text-xs font-semibold text-white"),
                    f"h-full bg-gradient-to-t from-purple-600 to-purple-400 rounded-t-lg flex items-end justify-center pb-2 transition-all duration-500"
                ),
                wv.span(category, "text-sm mt-2 text-gray-700 font-medium")
            ),
            f"flex flex-col items-center animate-scaleIn" ,
            style=f"animation-delay: {delay}s; height: {height_percent}%"
        )
        i += 1

    # Recent activity feed
    activity_items = ""
    i = 0
    for activity in recent_activity:
        delay = i * 0.05
        activity_items += wv.div(
            wv.render(
                wv.div(
                    wv.render(
                        wv.div(
                            wv.span("💳", "text-2xl"),
                            "w-12 h-12 rounded-full bg-blue-100 flex items-center justify-center"
                        ),
                        wv.div(
                            wv.render(
                                wv.p(activity["user"], "font-semibold text-gray-800"),
                                wv.p(activity["action"], "text-sm text-gray-600")
                            ),
                            "flex-1"
                        ),
                        wv.div(
                            wv.render(
                                wv.p(f"${activity['amount']}", "font-bold text-green-600"),
                                wv.p(activity["date"], "text-xs text-gray-500")
                            ),
                            "text-right"
                        )
                    ),
                    "flex items-center gap-4"
                )
            ),
            f"p-4 bg-white rounded-lg shadow-sm hover:shadow-md transition-all duration-300 animate-slideInLeft",
            style=f"animation-delay: {delay}s"
        )
        i += 1

    # Top products table
    products_rows = ""
    i = 0
    for product in top_products:
        delay = i * 0.1
        products_rows += wv.div(
            wv.render(
                wv.div(f"{i+1}", "w-8 h-8 rounded-full bg-purple-100 text-purple-600 font-bold flex items-center justify-center"),
                wv.div(
                    wv.render(
                        wv.p(product["name"], "font-semibold text-gray-800"),
                        wv.p(product["category"], "text-sm text-gray-500")
                    ),
                    "flex-1"
                ),
                wv.div(
                    wv.render(
                        wv.p(f"{product['sales']} units", "text-gray-700"),
                        wv.p(f"${product['revenue']}", "text-green-600 font-semibold")
                    ),
                    "text-right"
                )
            ),
            f"flex items-center gap-4 p-4 bg-white rounded-lg shadow-sm hover:shadow-md transition-all duration-300 animate-fadeIn",
            style=f"animation-delay: {delay}s"
        )

    # Statistics cards with animations
    stats_cards = wv.div(
        wv.render(
            # Revenue Card
            wv.div(
                wv.render(
                    wv.div(
                        wv.span("💰", "text-4xl"),
                        "w-16 h-16 rounded-full bg-gradient-to-br from-green-400 to-green-600 flex items-center justify-center shadow-lg"
                    ),
                    wv.div(
                        wv.render(
                            wv.p("Total Revenue", "text-gray-600 text-sm font-medium"),
                            wv.h2(f"${total_revenue:,.2f}", "text-3xl font-bold text-gray-800 mt-1"),
                            wv.p("+12.5% from last month", "text-green-600 text-sm mt-2 font-semibold")
                        ),
                        "flex-1"
                    )
                ),
                "flex items-center gap-4 p-6 bg-white rounded-xl shadow-lg hover:shadow-2xl transition-all duration-300 transform hover:-translate-y-1 animate-scaleIn",
                style="animation-delay: 0s"
            ),
            # Users Card
            wv.div(
                wv.render(
                    wv.div(
                        wv.span("👥", "text-4xl"),
                        "w-16 h-16 rounded-full bg-gradient-to-br from-blue-400 to-blue-600 flex items-center justify-center shadow-lg"
                    ),
                    wv.div(
                        wv.render(
                            wv.p("Active Users", "text-gray-600 text-sm font-medium"),
                            wv.h2(f"{active_users:,}", "text-3xl font-bold text-gray-800 mt-1"),
                            wv.p("+8.2% from last month", "text-blue-600 text-sm mt-2 font-semibold")
                        ),
                        "flex-1"
                    )
                ),
                "flex items-center gap-4 p-6 bg-white rounded-xl shadow-lg hover:shadow-2xl transition-all duration-300 transform hover:-translate-y-1 animate-scaleIn",
                style="animation-delay: 0.1s"
            ),
            # Orders Card
            wv.div(
                wv.render(
                    wv.div(
                        wv.span("📦", "text-4xl"),
                        "w-16 h-16 rounded-full bg-gradient-to-br from-purple-400 to-purple-600 flex items-center justify-center shadow-lg"
                    ),
                    wv.div(
                        wv.render(
                            wv.p("Total Orders", "text-gray-600 text-sm font-medium"),
                            wv.h2(f"{total_orders:,}", "text-3xl font-bold text-gray-800 mt-1"),
                            wv.p("+15.3% from last month", "text-purple-600 text-sm mt-2 font-semibold")
                        ),
                        "flex-1"
                    )
                ),
                "flex items-center gap-4 p-6 bg-white rounded-xl shadow-lg hover:shadow-2xl transition-all duration-300 transform hover:-translate-y-1 animate-scaleIn",
                style="animation-delay: 0.2s"
            ),
            # Average Order Card
            wv.div(
                wv.render(
                    wv.div(
                        wv.span("📊", "text-4xl"),
                        "w-16 h-16 rounded-full bg-gradient-to-br from-orange-400 to-orange-600 flex items-center justify-center shadow-lg"
                    ),
                    wv.div(
                        wv.render(
                            wv.p("Avg Order Value", "text-gray-600 text-sm font-medium"),
                            wv.h2(f"${avg_order_value:.2f}", "text-3xl font-bold text-gray-800 mt-1"),
                            wv.p("+3.7% from last month", "text-orange-600 text-sm mt-2 font-semibold")
                        ),
                        "flex-1"
                    )
                ),
                "flex items-center gap-4 p-6 bg-white rounded-xl shadow-lg hover:shadow-2xl transition-all duration-300 transform hover:-translate-y-1 animate-scaleIn",
                style="animation-delay: 0.3s"
            )
        ),
        "grid grid-cols-1 md:grid-cols-2 lg:grid-cols-4 gap-6 mb-8"
    )

    # Main dashboard layout
    dashboard_content = wv.div(
        wv.render(
            # Header
            wv.div(
                wv.render(
                    wv.h1("📈 Analytics Dashboard", "text-4xl font-bold text-gray-800 mb-2"),
                    wv.p("Full-stack application with Tauraro backend", "text-gray-600 text-lg")
                ),
                "mb-8 animate-fadeIn"
            ),

            # Statistics Cards
            stats_cards,

            # Charts and Activity Row
            wv.div(
                wv.render(
                    # Revenue by Category Chart
                    wv.div(
                        wv.render(
                            wv.h3("Revenue by Category", "text-2xl font-bold text-gray-800 mb-6"),
                            wv.div(
                                chart_bars,
                                "flex items-end justify-around gap-4 h-64 bg-gradient-to-br from-gray-50 to-gray-100 rounded-lg p-6"
                            )
                        ),
                        "bg-white p-6 rounded-xl shadow-lg animate-slideInLeft"
                    ),

                    # Recent Activity Feed
                    wv.div(
                        wv.render(
                            wv.h3("Recent Activity", "text-2xl font-bold text-gray-800 mb-6"),
                            wv.div(
                                activity_items,
                                "space-y-3 max-h-80 overflow-y-auto"
                            )
                        ),
                        "bg-white p-6 rounded-xl shadow-lg animate-slideInRight"
                    )
                ),
                "grid grid-cols-1 lg:grid-cols-2 gap-6 mb-8"
            ),

            # Top Products Table
            wv.div(
                wv.render(
                    wv.h3("🏆 Top Selling Products", "text-2xl font-bold text-gray-800 mb-6"),
                    wv.div(
                        products_rows,
                        "space-y-3"
                    )
                ),
                "bg-white p-6 rounded-xl shadow-lg animate-fadeIn",
                style="animation-delay: 0.4s"
            ),

            # Backend Info Footer
            wv.div(
                wv.render(
                    wv.p("🚀 Powered by Tauraro Backend", "text-center text-gray-600 font-semibold"),
                    wv.p(f"Data: {len(backend.users)} users | {len(backend.products)} products | {len(backend.transactions)} transactions",
                         "text-center text-gray-500 text-sm mt-2")
                ),
                "mt-8 p-6 bg-gradient-to-r from-purple-100 to-blue-100 rounded-xl animate-fadeIn",
                style="animation-delay: 0.5s"
            )
        ),
        "container mx-auto p-8 max-w-7xl"
    )

    return dashboard_content


def create_styles():
    """Create custom CSS styles"""
    return wv.style("""
        @keyframes fadeIn {
            from { opacity: 0; }
            to { opacity: 1; }
        }

        @keyframes slideInLeft {
            from {
                opacity: 0;
                transform: translateX(-30px);
            }
            to {
                opacity: 1;
                transform: translateX(0);
            }
        }

        @keyframes slideInRight {
            from {
                opacity: 0;
                transform: translateX(30px);
            }
            to {
                opacity: 1;
                transform: translateX(0);
            }
        }

        @keyframes scaleIn {
            from {
                opacity: 0;
                transform: scale(0.9);
            }
            to {
                opacity: 1;
                transform: scale(1);
            }
        }

        .animate-fadeIn {
            animation: fadeIn 0.6s ease-out forwards;
            opacity: 0;
        }

        .animate-slideInLeft {
            animation: slideInLeft 0.6s ease-out forwards;
            opacity: 0;
        }

        .animate-slideInRight {
            animation: slideInRight 0.6s ease-out forwards;
            opacity: 0;
        }

        .animate-scaleIn {
            animation: scaleIn 0.6s ease-out forwards;
            opacity: 0;
        }

        /* Custom scrollbar */
        ::-webkit-scrollbar {
            width: 8px;
        }

        ::-webkit-scrollbar-track {
            background: #f1f1f1;
            border-radius: 10px;
        }

        ::-webkit-scrollbar-thumb {
            background: #888;
            border-radius: 10px;
        }

        ::-webkit-scrollbar-thumb:hover {
            background: #555;
        }
    """)


# ============================================================================
# APPLICATION ENTRY POINT
# ============================================================================

def main():
    """Main application entry point"""

    print("=" * 70)
    print("  Full-Stack Analytics Dashboard with Tauraro Backend")
    print("=" * 70)
    print()

    # Initialize backend
    backend = AnalyticsBackend()
    # backend.initialize_data()

    print()
    print("[Frontend] Generating UI...")

    # Create UI with backend data
    dashboard_ui = create_dashboard_ui(backend)

    # Assemble complete HTML
    head_content = wv.render(
        wv.title("Analytics Dashboard - Tauraro"),
        wv.meta({"charset": "utf-8"}),
        wv.meta({"name": "viewport", "content": "width=device-width, initial-scale=1"}),
        wv.cdn_tailwind(),
        create_styles()
    )

    full_html = "<!DOCTYPE html>" + wv.html(
        wv.render(
            wv.head(head_content),
            wv.body(dashboard_ui, "bg-gradient-to-br from-gray-100 to-gray-200 min-h-screen")
        )
    )

    print("[Frontend] UI generated successfully")
    print()
    print("=" * 70)
    print("  Launching Dashboard Window...")
    print("=" * 70)
    print()

    # Create and launch window
    window = wv.Window("Analytics Dashboard - Full Stack with Tauraro", 1400, 900)
    window.set_html(full_html)
    window.run()  # Blocks until window closes

    print()
    print("Dashboard closed. Goodbye!")


# Run the application
if __name__ == "__main__":
    main()