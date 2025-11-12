"""
Comprehensive Dashboard Example with Animations and Responsive Design
Features: Charts, Cards, Tables, Animations, Mobile/Desktop Views
"""

import webviewtk as wv

def create_dashboard():
    tailwind = wv.cdn_tailwind()
    alpine = wv.cdn_alpine()

    # Custom CSS for animations and extra styling
    custom_css = """
        @keyframes fadeIn {
            from { opacity: 0; transform: translateY(20px); }
            to { opacity: 1; transform: translateY(0); }
        }

        @keyframes slideIn {
            from { opacity: 0; transform: translateX(-20px); }
            to { opacity: 1; transform: translateX(0); }
        }

        @keyframes pulse {
            0%, 100% { transform: scale(1); }
            50% { transform: scale(1.05); }
        }

        @keyframes spin {
            from { transform: rotate(0deg); }
            to { transform: rotate(360deg); }
        }

        .fade-in {
            animation: fadeIn 0.6s ease-out;
        }

        .slide-in {
            animation: slideIn 0.6s ease-out;
        }

        .card-hover {
            transition: all 0.3s ease;
        }

        .card-hover:hover {
            transform: translateY(-5px);
            box-shadow: 0 20px 25px -5px rgba(0, 0, 0, 0.1), 0 10px 10px -5px rgba(0, 0, 0, 0.04);
        }

        .stat-card {
            background: linear-gradient(135deg, var(--from-color) 0%, var(--to-color) 100%);
        }

        .progress-bar {
            transition: width 1s ease-out;
        }

        .chart-bar {
            transition: height 0.8s ease-out;
        }

        @media (max-width: 768px) {
            .mobile-nav {
                display: block;
            }
            .desktop-nav {
                display: none;
            }
        }

        @media (min-width: 769px) {
            .mobile-nav {
                display: none;
            }
            .desktop-nav {
                display: block;
            }
        }
    """

    # Alpine.js state management
    alpine_script = """
        <script>
            function dashboard() {
                return {
                    sidebarOpen: false,
                    currentPage: 'dashboard',
                    stats: {
                        users: 12543,
                        revenue: 89420,
                        orders: 3241,
                        growth: 23.5
                    },
                    chartData: [
                        { month: 'Jan', value: 30 },
                        { month: 'Feb', value: 45 },
                        { month: 'Mar', value: 60 },
                        { month: 'Apr', value: 55 },
                        { month: 'May', value: 70 },
                        { month: 'Jun', value: 85 }
                    ],
                    activities: [
                        { user: 'John Doe', action: 'Created new project', time: '2 min ago', type: 'success' },
                        { user: 'Jane Smith', action: 'Uploaded 5 files', time: '15 min ago', type: 'info' },
                        { user: 'Bob Johnson', action: 'Deleted account', time: '1 hour ago', type: 'danger' },
                        { user: 'Alice Brown', action: 'Updated profile', time: '3 hours ago', type: 'warning' }
                    ],
                    toggleSidebar() {
                        this.sidebarOpen = !this.sidebarOpen;
                    },
                    formatNumber(num) {
                        return num.toLocaleString();
                    },
                    getActivityColor(type) {
                        const colors = {
                            success: 'bg-green-500',
                            info: 'bg-blue-500',
                            danger: 'bg-red-500',
                            warning: 'bg-yellow-500'
                        };
                        return colors[type] || 'bg-gray-500';
                    }
                }
            }
        </script>
    """

    # Header with responsive navigation
    header = wv.header(
        wv.render(
            # Desktop Navigation
            wv.div(
                wv.render(
                    wv.div(
                        wv.h1("Dashboard Pro", "text-2xl font-bold text-white"),
                        "flex items-center"
                    ),
                    wv.nav(
                        wv.render(
                            wv.link("Dashboard", "#", "text-white hover:text-blue-200 px-4 py-2"),
                            wv.link("Analytics", "#", "text-white hover:text-blue-200 px-4 py-2"),
                            wv.link("Reports", "#", "text-white hover:text-blue-200 px-4 py-2"),
                            wv.link("Settings", "#", "text-white hover:text-blue-200 px-4 py-2")
                        ),
                        "flex space-x-2"
                    ),
                    wv.div(
                        wv.button("Profile", "bg-white text-blue-600 px-4 py-2 rounded-lg hover:bg-blue-50"),
                        "flex items-center"
                    )
                ),
                "desktop-nav container mx-auto flex justify-between items-center"
            ),
            # Mobile Navigation
            wv.div(
                wv.render(
                    wv.div(
                        wv.render(
                            wv.button("☰", "text-white text-2xl px-4 py-2 @click='toggleSidebar()'"),
                            wv.h1("Dashboard Pro", "text-xl font-bold text-white")
                        ),
                        "flex items-center space-x-4"
                    ),
                    wv.button("Profile", "bg-white text-blue-600 px-3 py-1 rounded text-sm")
                ),
                "mobile-nav container mx-auto flex justify-between items-center"
            )
        ),
        "bg-gradient-to-r from-blue-600 to-purple-600 py-4 shadow-lg fade-in"
    )

    # Stats Cards
    stats_section = wv.div(
        wv.render(
            # Users Card
            wv.div(
                wv.render(
                    wv.div(
                        wv.render(
                            wv.div(
                                wv.render(
                                    wv.h3("Total Users", "text-white text-sm font-medium opacity-90"),
                                    wv.p("", "text-white text-3xl font-bold mt-2", "", "", {"x-text": "formatNumber(stats.users)"})
                                ),
                                ""
                            ),
                            wv.div("👥", "text-4xl opacity-80")
                        ),
                        "flex justify-between items-start"
                    ),
                    wv.div(
                        wv.span("+12.5%", "text-white text-sm opacity-90"),
                        "mt-4"
                    )
                ),
                "stat-card card-hover p-6 rounded-xl shadow-lg text-white fade-in",
                "",
                "style='--from-color: #3B82F6; --to-color: #8B5CF6; animation-delay: 0.1s'"
            ),
            # Revenue Card
            wv.div(
                wv.render(
                    wv.div(
                        wv.render(
                            wv.div(
                                wv.render(
                                    wv.h3("Revenue", "text-white text-sm font-medium opacity-90"),
                                    wv.p("", "text-white text-3xl font-bold mt-2", "", "", {"x-text": "'$' + formatNumber(stats.revenue)"})
                                ),
                                ""
                            ),
                            wv.div("💰", "text-4xl opacity-80")
                        ),
                        "flex justify-between items-start"
                    ),
                    wv.div(
                        wv.span("+23.5%", "text-white text-sm opacity-90"),
                        "mt-4"
                    )
                ),
                "stat-card card-hover p-6 rounded-xl shadow-lg text-white fade-in",
                "",
                "style='--from-color: #10B981; --to-color: #059669; animation-delay: 0.2s'"
            ),
            # Orders Card
            wv.div(
                wv.render(
                    wv.div(
                        wv.render(
                            wv.div(
                                wv.render(
                                    wv.h3("Orders", "text-white text-sm font-medium opacity-90"),
                                    wv.p("", "text-white text-3xl font-bold mt-2", "", "", {"x-text": "formatNumber(stats.orders)"})
                                ),
                                ""
                            ),
                            wv.div("📦", "text-4xl opacity-80")
                        ),
                        "flex justify-between items-start"
                    ),
                    wv.div(
                        wv.span("+8.2%", "text-white text-sm opacity-90"),
                        "mt-4"
                    )
                ),
                "stat-card card-hover p-6 rounded-xl shadow-lg text-white fade-in",
                "",
                "style='--from-color: #F59E0B; --to-color: #D97706; animation-delay: 0.3s'"
            ),
            # Growth Card
            wv.div(
                wv.render(
                    wv.div(
                        wv.render(
                            wv.div(
                                wv.render(
                                    wv.h3("Growth", "text-white text-sm font-medium opacity-90"),
                                    wv.p("", "text-white text-3xl font-bold mt-2", "", "", {"x-text": "stats.growth + '%'"})
                                ),
                                ""
                            ),
                            wv.div("📈", "text-4xl opacity-80")
                        ),
                        "flex justify-between items-start"
                    ),
                    wv.div(
                        wv.span("Last 30 days", "text-white text-sm opacity-90"),
                        "mt-4"
                    )
                ),
                "stat-card card-hover p-6 rounded-xl shadow-lg text-white fade-in",
                "",
                "style='--from-color: #EF4444; --to-color: #DC2626; animation-delay: 0.4s'"
            )
        ),
        "grid grid-cols-1 md:grid-cols-2 lg:grid-cols-4 gap-6 mb-8"
    )

    # Chart Section
    chart_section = wv.div(
        wv.render(
            wv.div(
                wv.render(
                    wv.h2("Revenue Overview", "text-2xl font-bold text-gray-800 mb-6"),
                    wv.div(
                        wv.render(
                            # Chart bars
                            """
                            <template x-for="(item, index) in chartData" :key="index">
                                <div class="flex flex-col items-center">
                                    <div class="w-12 md:w-16 bg-gradient-to-t from-blue-500 to-purple-500 rounded-t-lg chart-bar"
                                         :style="'height: ' + item.value + '%; animation-delay: ' + (index * 0.1) + 's'">
                                    </div>
                                    <span class="text-xs text-gray-600 mt-2" x-text="item.month"></span>
                                </div>
                            </template>
                            """
                        ),
                        "flex justify-between items-end h-64 px-4"
                    )
                ),
                "bg-white p-6 md:p-8 rounded-xl shadow-lg slide-in"
            )
        ),
        "mb-8"
    )

    # Activity Feed and Quick Actions
    bottom_section = wv.div(
        wv.render(
            # Activity Feed
            wv.div(
                wv.render(
                    wv.h2("Recent Activity", "text-xl font-bold text-gray-800 mb-4"),
                    wv.div(
                        """
                        <template x-for="(activity, index) in activities" :key="index">
                            <div class="flex items-start space-x-4 p-4 hover:bg-gray-50 rounded-lg transition cursor-pointer"
                                 :style="'animation: fadeIn 0.6s ease-out; animation-delay: ' + (index * 0.1) + 's'">
                                <div :class="getActivityColor(activity.type)" class="w-2 h-2 rounded-full mt-2"></div>
                                <div class="flex-1">
                                    <p class="text-sm font-medium text-gray-900" x-text="activity.user"></p>
                                    <p class="text-sm text-gray-600" x-text="activity.action"></p>
                                    <p class="text-xs text-gray-400 mt-1" x-text="activity.time"></p>
                                </div>
                            </div>
                        </template>
                        """,
                        "space-y-2"
                    )
                ),
                "bg-white p-6 rounded-xl shadow-lg"
            ),
            # Quick Actions
            wv.div(
                wv.render(
                    wv.h2("Quick Actions", "text-xl font-bold text-gray-800 mb-4"),
                    wv.div(
                        wv.render(
                            wv.button(
                                wv.render(
                                    wv.span("➕", "text-2xl"),
                                    wv.span("New Project", "mt-2 text-sm")
                                ),
                                "flex flex-col items-center justify-center p-6 bg-gradient-to-br from-blue-500 to-blue-600 text-white rounded-xl hover:from-blue-600 hover:to-blue-700 transition card-hover"
                            ),
                            wv.button(
                                wv.render(
                                    wv.span("📊", "text-2xl"),
                                    wv.span("View Reports", "mt-2 text-sm")
                                ),
                                "flex flex-col items-center justify-center p-6 bg-gradient-to-br from-green-500 to-green-600 text-white rounded-xl hover:from-green-600 hover:to-green-700 transition card-hover"
                            ),
                            wv.button(
                                wv.render(
                                    wv.span("⚙️", "text-2xl"),
                                    wv.span("Settings", "mt-2 text-sm")
                                ),
                                "flex flex-col items-center justify-center p-6 bg-gradient-to-br from-purple-500 to-purple-600 text-white rounded-xl hover:from-purple-600 hover:to-purple-700 transition card-hover"
                            ),
                            wv.button(
                                wv.render(
                                    wv.span("👥", "text-2xl"),
                                    wv.span("Team", "mt-2 text-sm")
                                ),
                                "flex flex-col items-center justify-center p-6 bg-gradient-to-br from-orange-500 to-orange-600 text-white rounded-xl hover:from-orange-600 hover:to-orange-700 transition card-hover"
                            )
                        ),
                        "grid grid-cols-2 gap-4"
                    )
                ),
                "bg-white p-6 rounded-xl shadow-lg"
            )
        ),
        "grid grid-cols-1 lg:grid-cols-2 gap-8"
    )

    # Main content
    main_content = wv.div(
        wv.render(
            header,
            wv.div(
                wv.render(
                    stats_section,
                    chart_section,
                    bottom_section
                ),
                "container mx-auto px-4 py-8"
            )
        ),
        "", "", "", {"x-data": "dashboard()"}
    )

    # Build complete HTML
    head_content = wv.render(
        wv.title("Dashboard Pro - Tauraro WebViewTK"),
        wv.meta({"charset": "utf-8"}),
        wv.meta({"name": "viewport", "content": "width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no"}),
        wv.style(custom_css),
        tailwind,
        alpine,
        alpine_script
    )

    body_content = main_content

    full_html = "<!DOCTYPE html>" + wv.html(
        wv.render(
            wv.head(head_content),
            wv.body(body_content, "bg-gray-50 min-h-screen")
        )
    )

    return full_html

# Create and display the dashboard
print("Creating comprehensive dashboard with animations...")
window = wv.Window("Dashboard Pro - Tauraro WebViewTK", 1400, 900)
window.set_html(create_dashboard())
print("Dashboard created! Launching window...")
window.run()
