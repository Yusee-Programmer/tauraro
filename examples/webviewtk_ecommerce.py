"""
E-Commerce Store Example with Shopping Cart and Product Gallery
Features: Product cards, Shopping cart, Filters, Mobile-responsive design
"""

import webviewtk as wv

def create_ecommerce_store():
    tailwind = wv.cdn_tailwind()
    alpine = wv.cdn_alpine()

    # Custom animations and styles
    custom_css = """
        @keyframes fadeInUp {
            from {
                opacity: 0;
                transform: translateY(30px);
            }
            to {
                opacity: 1;
                transform: translateY(0);
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

        @keyframes slideInRight {
            from {
                transform: translateX(100%);
            }
            to {
                transform: translateX(0);
            }
        }

        .fade-in-up {
            animation: fadeInUp 0.6s ease-out;
        }

        .scale-in {
            animation: scaleIn 0.4s ease-out;
        }

        .product-card {
            transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
        }

        .product-card:hover {
            transform: translateY(-10px);
            box-shadow: 0 25px 50px -12px rgba(0, 0, 0, 0.25);
        }

        .product-image {
            transition: transform 0.5s ease;
        }

        .product-card:hover .product-image {
            transform: scale(1.1);
        }

        .cart-badge {
            animation: scaleIn 0.3s ease-out;
        }

        .cart-drawer {
            animation: slideInRight 0.3s ease-out;
        }

        @media (max-width: 768px) {
            .mobile-menu {
                display: block;
            }
            .desktop-menu {
                display: none;
            }
        }

        @media (min-width: 769px) {
            .mobile-menu {
                display: none;
            }
            .desktop-menu {
                display: block;
            }
        }
    """

    # Alpine.js store logic
    alpine_script = """
        <script>
            function store() {
                return {
                    cartOpen: false,
                    selectedCategory: 'all',
                    searchQuery: '',
                    cart: [],
                    products: [
                        {
                            id: 1,
                            name: 'Premium Headphones',
                            category: 'electronics',
                            price: 299.99,
                            image: '🎧',
                            rating: 4.8,
                            reviews: 234,
                            description: 'High-quality wireless headphones with noise cancellation'
                        },
                        {
                            id: 2,
                            name: 'Smart Watch Pro',
                            category: 'electronics',
                            price: 399.99,
                            image: '⌚',
                            rating: 4.9,
                            reviews: 456,
                            description: 'Advanced fitness tracking and health monitoring'
                        },
                        {
                            id: 3,
                            name: 'Designer Backpack',
                            category: 'fashion',
                            price: 129.99,
                            image: '🎒',
                            rating: 4.7,
                            reviews: 189,
                            description: 'Stylish and durable backpack for daily use'
                        },
                        {
                            id: 4,
                            name: 'Running Shoes',
                            category: 'sports',
                            price: 159.99,
                            image: '👟',
                            rating: 4.6,
                            reviews: 321,
                            description: 'Comfortable running shoes for all terrains'
                        },
                        {
                            id: 5,
                            name: 'Coffee Maker',
                            category: 'home',
                            price: 89.99,
                            image: '☕',
                            rating: 4.5,
                            reviews: 145,
                            description: 'Automatic coffee maker with timer'
                        },
                        {
                            id: 6,
                            name: 'Yoga Mat',
                            category: 'sports',
                            price: 49.99,
                            image: '🧘',
                            rating: 4.8,
                            reviews: 267,
                            description: 'Non-slip premium yoga mat'
                        },
                        {
                            id: 7,
                            name: 'Laptop Sleeve',
                            category: 'electronics',
                            price: 39.99,
                            image: '💼',
                            rating: 4.4,
                            reviews: 98,
                            description: 'Protective laptop sleeve up to 15 inches'
                        },
                        {
                            id: 8,
                            name: 'Sunglasses',
                            category: 'fashion',
                            price: 79.99,
                            image: '🕶️',
                            rating: 4.6,
                            reviews: 178,
                            description: 'UV protection polarized sunglasses'
                        }
                    ],
                    get filteredProducts() {
                        let filtered = this.products;

                        if (this.selectedCategory !== 'all') {
                            filtered = filtered.filter(p => p.category === this.selectedCategory);
                        }

                        if (this.searchQuery) {
                            filtered = filtered.filter(p =>
                                p.name.toLowerCase().includes(this.searchQuery.toLowerCase()) ||
                                p.description.toLowerCase().includes(this.searchQuery.toLowerCase())
                            );
                        }

                        return filtered;
                    },
                    get cartTotal() {
                        return this.cart.reduce((sum, item) => sum + (item.price * item.quantity), 0);
                    },
                    get cartCount() {
                        return this.cart.reduce((sum, item) => sum + item.quantity, 0);
                    },
                    addToCart(product) {
                        const existing = this.cart.find(item => item.id === product.id);
                        if (existing) {
                            existing.quantity++;
                        } else {
                            this.cart.push({ ...product, quantity: 1 });
                        }
                        this.showNotification('Added to cart!');
                    },
                    removeFromCart(productId) {
                        this.cart = this.cart.filter(item => item.id !== productId);
                    },
                    updateQuantity(productId, change) {
                        const item = this.cart.find(item => item.id === productId);
                        if (item) {
                            item.quantity += change;
                            if (item.quantity <= 0) {
                                this.removeFromCart(productId);
                            }
                        }
                    },
                    toggleCart() {
                        this.cartOpen = !this.cartOpen;
                    },
                    formatPrice(price) {
                        return '$' + price.toFixed(2);
                    },
                    showNotification(message) {
                        // Simple notification feedback
                        console.log(message);
                    }
                }
            }
        </script>
    """

    # Header with cart
    header = wv.header(
        wv.render(
            wv.div(
                wv.render(
                    # Logo and brand
                    wv.div(
                        wv.render(
                            wv.span("🛍️", "text-3xl"),
                            wv.span("TechStore", "text-2xl font-bold text-white ml-2")
                        ),
                        "flex items-center"
                    ),
                    # Search bar (desktop)
                    wv.div(
                        wv.input(
                            "text",
                            "Search products...",
                            "w-full px-4 py-2 rounded-lg focus:outline-none focus:ring-2 focus:ring-white",
                            "",
                            "",
                            {"x-model": "searchQuery"}
                        ),
                        "desktop-menu flex-1 max-w-md mx-8"
                    ),
                    # Cart button
                    wv.div(
                        wv.render(
                            wv.button(
                                wv.render(
                                    wv.span("🛒", "text-2xl"),
                                    """<span x-show="cartCount > 0"
                                            class="cart-badge absolute -top-2 -right-2 bg-red-500 text-white text-xs rounded-full w-6 h-6 flex items-center justify-center font-bold"
                                            x-text="cartCount"></span>"""
                                ),
                                "relative p-2 hover:bg-white hover:bg-opacity-20 rounded-lg transition",
                                "",
                                "",
                                {"@click": "toggleCart()"}
                            )
                        ),
                        "flex items-center"
                    )
                ),
                "container mx-auto px-4 py-4 flex items-center justify-between"
            )
        ),
        "bg-gradient-to-r from-purple-600 to-blue-600 shadow-lg sticky top-0 z-50 fade-in-up"
    )

    # Mobile search
    mobile_search = wv.div(
        wv.input(
            "text",
            "Search products...",
            "w-full px-4 py-2 rounded-lg focus:outline-none focus:ring-2 focus:ring-purple-500",
            "",
            "",
            {"x-model": "searchQuery"}
        ),
        "mobile-menu px-4 py-3 bg-white shadow"
    )

    # Category filters
    category_filters = wv.div(
        wv.render(
            wv.h2("Categories", "text-sm font-semibold text-gray-600 uppercase mb-3"),
            wv.div(
                wv.render(
                    wv.button("All", "px-4 py-2 rounded-lg transition", "", "",
                             {"@click": "selectedCategory = 'all'",
                              ":class": "selectedCategory === 'all' ? 'bg-purple-600 text-white' : 'bg-gray-200 text-gray-700 hover:bg-gray-300'"}),
                    wv.button("Electronics", "px-4 py-2 rounded-lg transition", "", "",
                             {"@click": "selectedCategory = 'electronics'",
                              ":class": "selectedCategory === 'electronics' ? 'bg-purple-600 text-white' : 'bg-gray-200 text-gray-700 hover:bg-gray-300'"}),
                    wv.button("Fashion", "px-4 py-2 rounded-lg transition", "", "",
                             {"@click": "selectedCategory = 'fashion'",
                              ":class": "selectedCategory === 'fashion' ? 'bg-purple-600 text-white' : 'bg-gray-200 text-gray-700 hover:bg-gray-300'"}),
                    wv.button("Sports", "px-4 py-2 rounded-lg transition", "", "",
                             {"@click": "selectedCategory = 'sports'",
                              ":class": "selectedCategory === 'sports' ? 'bg-purple-600 text-white' : 'bg-gray-200 text-gray-700 hover:bg-gray-300'"}),
                    wv.button("Home", "px-4 py-2 rounded-lg transition", "", "",
                             {"@click": "selectedCategory = 'home'",
                              ":class": "selectedCategory === 'home' ? 'bg-purple-600 text-white' : 'bg-gray-200 text-gray-700 hover:bg-gray-300'"})
                ),
                "flex flex-wrap gap-2"
            )
        ),
        "px-4 py-6"
    )

    # Product grid
    product_grid = wv.div(
        """
        <template x-for="(product, index) in filteredProducts" :key="product.id">
            <div class="product-card bg-white rounded-xl shadow-lg overflow-hidden scale-in"
                 :style="'animation-delay: ' + (index * 0.1) + 's'">
                <div class="overflow-hidden bg-gradient-to-br from-purple-100 to-blue-100 h-48 flex items-center justify-center">
                    <span class="product-image text-7xl" x-text="product.image"></span>
                </div>
                <div class="p-5">
                    <h3 class="text-lg font-bold text-gray-800 mb-2" x-text="product.name"></h3>
                    <p class="text-sm text-gray-600 mb-3 h-10 overflow-hidden" x-text="product.description"></p>
                    <div class="flex items-center mb-3">
                        <span class="text-yellow-500">⭐</span>
                        <span class="text-sm font-medium ml-1" x-text="product.rating"></span>
                        <span class="text-xs text-gray-500 ml-1">(<span x-text="product.reviews"></span>)</span>
                    </div>
                    <div class="flex items-center justify-between">
                        <span class="text-2xl font-bold text-purple-600" x-text="formatPrice(product.price)"></span>
                        <button @click="addToCart(product)"
                                class="bg-purple-600 hover:bg-purple-700 text-white px-4 py-2 rounded-lg transition transform hover:scale-105">
                            Add to Cart
                        </button>
                    </div>
                </div>
            </div>
        </template>
        """,
        "grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 xl:grid-cols-4 gap-6 px-4 py-6"
    )

    # Shopping cart drawer
    cart_drawer = wv.div(
        wv.render(
            # Overlay
            """<div x-show="cartOpen" @click="cartOpen = false"
                  x-transition:enter="transition ease-out duration-300"
                  x-transition:enter-start="opacity-0"
                  x-transition:enter-end="opacity-100"
                  x-transition:leave="transition ease-in duration-200"
                  x-transition:leave-start="opacity-100"
                  x-transition:leave-end="opacity-0"
                  class="fixed inset-0 bg-black bg-opacity-50 z-40"></div>""",
            # Cart panel
            wv.div(
                wv.render(
                    # Header
                    wv.div(
                        wv.render(
                            wv.h2("Shopping Cart", "text-2xl font-bold text-gray-800"),
                            wv.button("✕", "text-2xl text-gray-600 hover:text-gray-800", "", "", {"@click": "cartOpen = false"})
                        ),
                        "flex justify-between items-center p-6 border-b"
                    ),
                    # Cart items
                    wv.div(
                        """
                        <div x-show="cart.length === 0" class="text-center py-12">
                            <span class="text-6xl">🛒</span>
                            <p class="text-gray-600 mt-4">Your cart is empty</p>
                        </div>
                        <template x-for="item in cart" :key="item.id">
                            <div class="p-4 border-b hover:bg-gray-50 transition">
                                <div class="flex items-start space-x-4">
                                    <div class="text-4xl" x-text="item.image"></div>
                                    <div class="flex-1">
                                        <h4 class="font-semibold text-gray-800" x-text="item.name"></h4>
                                        <p class="text-sm text-purple-600 font-medium" x-text="formatPrice(item.price)"></p>
                                        <div class="flex items-center mt-2 space-x-2">
                                            <button @click="updateQuantity(item.id, -1)"
                                                    class="w-8 h-8 rounded-full bg-gray-200 hover:bg-gray-300 flex items-center justify-center">
                                                -
                                            </button>
                                            <span class="w-8 text-center font-medium" x-text="item.quantity"></span>
                                            <button @click="updateQuantity(item.id, 1)"
                                                    class="w-8 h-8 rounded-full bg-gray-200 hover:bg-gray-300 flex items-center justify-center">
                                                +
                                            </button>
                                        </div>
                                    </div>
                                    <button @click="removeFromCart(item.id)"
                                            class="text-red-500 hover:text-red-700">
                                        🗑️
                                    </button>
                                </div>
                            </div>
                        </template>
                        """,
                        "flex-1 overflow-y-auto"
                    ),
                    # Footer with total
                    """<div x-show="cart.length > 0" class="p-6 border-t bg-gray-50">
                        <div class="flex justify-between items-center mb-4">
                            <span class="text-lg font-semibold">Total:</span>
                            <span class="text-2xl font-bold text-purple-600" x-text="formatPrice(cartTotal)"></span>
                        </div>
                        <button class="w-full bg-purple-600 hover:bg-purple-700 text-white py-3 rounded-lg font-semibold transition transform hover:scale-105">
                            Checkout
                        </button>
                    </div>"""
                ),
                "cart-drawer fixed right-0 top-0 h-full w-full sm:w-96 bg-white shadow-2xl z-50 flex flex-col",
                "",
                "",
                {"x-show": "cartOpen", "x-transition:enter": "transition ease-out duration-300",
                 "x-transition:enter-start": "translate-x-full", "x-transition:enter-end": "translate-x-0",
                 "x-transition:leave": "transition ease-in duration-200",
                 "x-transition:leave-start": "translate-x-0", "x-transition:leave-end": "translate-x-full"}
            )
        ),
        ""
    )

    # Main content
    main_content = wv.div(
        wv.render(
            header,
            mobile_search,
            category_filters,
            product_grid,
            cart_drawer
        ),
        "", "", "", {"x-data": "store()"}
    )

    # Build complete HTML
    head_content = wv.render(
        wv.title("TechStore - E-Commerce Demo"),
        wv.meta({"charset": "utf-8"}),
        wv.meta({"name": "viewport", "content": "width=device-width, initial-scale=1, maximum-scale=1"}),
        wv.style(custom_css),
        tailwind,
        alpine,
        alpine_script
    )

    full_html = "<!DOCTYPE html>" + wv.html(
        wv.render(
            wv.head(head_content),
            wv.body(main_content, "bg-gray-100 min-h-screen")
        )
    )

    return full_html

# Create and display the store
print("Creating e-commerce store with shopping cart...")
window = wv.Window("TechStore - E-Commerce Demo", 1400, 900)
window.set_html(create_ecommerce_store())
print("Store created! Launching window...")
window.run()
