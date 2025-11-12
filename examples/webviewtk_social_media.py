"""
Social Media Feed Example with Posts, Likes, Comments, and Stories
Features: Interactive posts, Stories carousel, Like animations, Mobile-first design
"""

import webviewtk as wv

def create_social_feed():
    tailwind = wv.cdn_tailwind()
    alpine = wv.cdn_alpine()

    # Custom animations and styles
    custom_css = """
        @keyframes heartBeat {
            0%, 100% { transform: scale(1); }
            10%, 30% { transform: scale(0.9); }
            20%, 40% { transform: scale(1.1); }
        }

        @keyframes slideUp {
            from {
                opacity: 0;
                transform: translateY(20px);
            }
            to {
                opacity: 1;
                transform: translateY(0);
            }
        }

        @keyframes fadeIn {
            from { opacity: 0; }
            to { opacity: 1; }
        }

        @keyframes shimmer {
            0% { background-position: -1000px 0; }
            100% { background-position: 1000px 0; }
        }

        .heart-animation {
            animation: heartBeat 0.5s ease-in-out;
        }

        .slide-up {
            animation: slideUp 0.4s ease-out;
        }

        .story-ring {
            background: linear-gradient(45deg, #f09433 0%, #e6683c 25%, #dc2743 50%, #cc2366 75%, #bc1888 100%);
            padding: 3px;
            border-radius: 50%;
        }

        .post-card {
            transition: all 0.3s ease;
        }

        .post-card:hover {
            box-shadow: 0 10px 30px rgba(0, 0, 0, 0.15);
        }

        .skeleton {
            background: linear-gradient(90deg, #f0f0f0 25%, #e0e0e0 50%, #f0f0f0 75%);
            background-size: 1000px 100%;
            animation: shimmer 2s infinite;
        }

        .comment-input:focus {
            outline: none;
            border-color: #3B82F6;
        }

        @media (max-width: 768px) {
            .desktop-sidebar {
                display: none;
            }
        }

        @media (min-width: 769px) {
            .mobile-nav {
                display: none;
            }
        }
    """

    # Alpine.js social feed logic
    alpine_script = """
        <script>
            function socialFeed() {
                return {
                    currentUser: {
                        name: 'You',
                        username: '@you',
                        avatar: '👤'
                    },
                    stories: [
                        { id: 1, user: 'John', avatar: '👨', viewed: false },
                        { id: 2, user: 'Emma', avatar: '👩', viewed: false },
                        { id: 3, user: 'Mike', avatar: '🧑', viewed: true },
                        { id: 4, user: 'Sarah', avatar: '👧', viewed: false },
                        { id: 5, user: 'Tom', avatar: '🧔', viewed: true },
                        { id: 6, user: 'Lisa', avatar: '👱‍♀️', viewed: false }
                    ],
                    posts: [
                        {
                            id: 1,
                            user: 'John Doe',
                            username: '@johndoe',
                            avatar: '👨',
                            time: '2 hours ago',
                            content: 'Just launched my new project! 🚀 Check it out and let me know what you think!',
                            image: '🖼️',
                            likes: 234,
                            comments: [],
                            liked: false,
                            bookmarked: false,
                            showComments: false
                        },
                        {
                            id: 2,
                            user: 'Emma Wilson',
                            username: '@emmaw',
                            avatar: '👩',
                            time: '4 hours ago',
                            content: 'Beautiful sunset at the beach today! 🌅 Nature never stops amazing me.',
                            image: '🌅',
                            likes: 456,
                            comments: [
                                { user: 'Mike', avatar: '🧑', text: 'Stunning view! 😍' },
                                { user: 'Sarah', avatar: '👧', text: 'Wish I was there!' }
                            ],
                            liked: true,
                            bookmarked: false,
                            showComments: false
                        },
                        {
                            id: 3,
                            user: 'Tech News',
                            username: '@technews',
                            avatar: '📱',
                            time: '6 hours ago',
                            content: 'Breaking: New AI technology revolutionizes software development! Read more at link in bio. 🤖💻',
                            image: '💻',
                            likes: 1289,
                            comments: [
                                { user: 'Alex', avatar: '👨‍💻', text: 'This is game-changing!' },
                                { user: 'Lisa', avatar: '👱‍♀️', text: 'Can\'t wait to try it out!' },
                                { user: 'Tom', avatar: '🧔', text: 'The future is here!' }
                            ],
                            liked: false,
                            bookmarked: true,
                            showComments: false
                        },
                        {
                            id: 4,
                            user: 'Fitness Pro',
                            username: '@fitnesspro',
                            avatar: '💪',
                            time: '8 hours ago',
                            content: 'Morning workout complete! ✅ Remember: consistency is key. What\'s your fitness goal for this week?',
                            image: '🏋️',
                            likes: 567,
                            comments: [],
                            liked: false,
                            bookmarked: false,
                            showComments: false
                        }
                    ],
                    newComment: {},
                    toggleLike(post) {
                        post.liked = !post.liked;
                        post.likes += post.liked ? 1 : -1;

                        // Trigger animation
                        if (post.liked) {
                            const likeBtn = event.target.closest('.like-btn');
                            if (likeBtn) {
                                likeBtn.classList.add('heart-animation');
                                setTimeout(() => likeBtn.classList.remove('heart-animation'), 500);
                            }
                        }
                    },
                    toggleBookmark(post) {
                        post.bookmarked = !post.bookmarked;
                    },
                    toggleComments(post) {
                        post.showComments = !post.showComments;
                    },
                    addComment(post) {
                        if (this.newComment[post.id] && this.newComment[post.id].trim()) {
                            post.comments.push({
                                user: this.currentUser.name,
                                avatar: this.currentUser.avatar,
                                text: this.newComment[post.id]
                            });
                            this.newComment[post.id] = '';
                        }
                    },
                    formatLikes(count) {
                        if (count >= 1000) {
                            return (count / 1000).toFixed(1) + 'K';
                        }
                        return count.toString();
                    },
                    viewStory(story) {
                        story.viewed = true;
                    }
                }
            }
        </script>
    """

    # Top navigation bar
    nav_bar = wv.div(
        wv.render(
            wv.div(
                wv.render(
                    # Logo
                    wv.div(
                        wv.render(
                            wv.span("📱", "text-2xl"),
                            wv.span("SocialHub", "text-xl font-bold text-gray-800 ml-2 hidden md:inline")
                        ),
                        "flex items-center"
                    ),
                    # Search bar (desktop)
                    wv.div(
                        wv.input(
                            "text",
                            "Search...",
                            "w-full px-4 py-2 bg-gray-100 rounded-full focus:outline-none focus:ring-2 focus:ring-blue-500",
                            "",
                            ""
                        ),
                        "hidden md:block flex-1 max-w-md mx-8"
                    ),
                    # Navigation icons
                    wv.div(
                        wv.render(
                            wv.button("🏠", "text-2xl hover:bg-gray-100 p-2 rounded-full transition"),
                            wv.button("💬", "text-2xl hover:bg-gray-100 p-2 rounded-full transition"),
                            wv.button("❤️", "text-2xl hover:bg-gray-100 p-2 rounded-full transition"),
                            wv.button("👤", "text-2xl hover:bg-gray-100 p-2 rounded-full transition")
                        ),
                        "flex items-center space-x-2"
                    )
                ),
                "container mx-auto px-4 py-3 flex items-center justify-between"
            )
        ),
        "bg-white border-b sticky top-0 z-50 shadow-sm slide-up"
    )

    # Stories section
    stories_section = wv.div(
        wv.render(
            wv.h2("Stories", "text-sm font-semibold text-gray-600 mb-3 px-4"),
            wv.div(
                """
                <template x-for="story in stories" :key="story.id">
                    <button @click="viewStory(story)"
                            class="flex flex-col items-center space-y-2 flex-shrink-0 transition transform hover:scale-110">
                        <div :class="story.viewed ? 'ring-2 ring-gray-300' : 'story-ring'">
                            <div class="w-16 h-16 bg-white rounded-full flex items-center justify-center">
                                <span class="text-3xl" x-text="story.avatar"></span>
                            </div>
                        </div>
                        <span class="text-xs text-gray-600 max-w-[60px] truncate" x-text="story.user"></span>
                    </button>
                </template>
                """,
                "flex space-x-4 overflow-x-auto pb-4 px-4 scrollbar-hide"
            )
        ),
        "bg-white border-b py-4 mb-4"
    )

    # Posts feed
    posts_feed = wv.div(
        """
        <template x-for="(post, index) in posts" :key="post.id">
            <div class="post-card bg-white rounded-lg shadow-sm mb-4 overflow-hidden slide-up"
                 :style="'animation-delay: ' + (index * 0.1) + 's'">
                <!-- Post header -->
                <div class="flex items-center justify-between p-4">
                    <div class="flex items-center space-x-3">
                        <div class="w-10 h-10 rounded-full bg-gradient-to-br from-purple-400 to-blue-400 flex items-center justify-center">
                            <span class="text-2xl" x-text="post.avatar"></span>
                        </div>
                        <div>
                            <p class="font-semibold text-gray-800" x-text="post.user"></p>
                            <p class="text-xs text-gray-500" x-text="post.time"></p>
                        </div>
                    </div>
                    <button class="text-gray-600 hover:text-gray-800 text-xl">⋯</button>
                </div>

                <!-- Post content -->
                <div class="px-4 pb-2">
                    <p class="text-gray-800" x-text="post.content"></p>
                </div>

                <!-- Post image -->
                <div class="w-full bg-gradient-to-br from-blue-100 to-purple-100 flex items-center justify-center py-20">
                    <span class="text-8xl" x-text="post.image"></span>
                </div>

                <!-- Action buttons -->
                <div class="flex items-center justify-between px-4 py-3">
                    <div class="flex items-center space-x-4">
                        <button @click="toggleLike(post)" class="like-btn flex items-center space-x-1 transition">
                            <span :class="post.liked ? 'text-red-500' : 'text-gray-600'" class="text-2xl">
                                <span x-show="post.liked">❤️</span>
                                <span x-show="!post.liked">🤍</span>
                            </span>
                            <span class="text-sm font-medium" x-text="formatLikes(post.likes)"></span>
                        </button>
                        <button @click="toggleComments(post)" class="flex items-center space-x-1 text-gray-600 hover:text-blue-500 transition">
                            <span class="text-2xl">💬</span>
                            <span class="text-sm font-medium" x-text="post.comments.length"></span>
                        </button>
                        <button class="text-gray-600 hover:text-blue-500 transition">
                            <span class="text-2xl">📤</span>
                        </button>
                    </div>
                    <button @click="toggleBookmark(post)" class="transition">
                        <span :class="post.bookmarked ? 'text-yellow-500' : 'text-gray-600'" class="text-2xl">
                            <span x-show="post.bookmarked">🔖</span>
                            <span x-show="!post.bookmarked">📑</span>
                        </span>
                    </button>
                </div>

                <!-- Comments section -->
                <div x-show="post.showComments" class="border-t px-4 py-3 space-y-3"
                     x-transition:enter="transition ease-out duration-200"
                     x-transition:enter-start="opacity-0 -translate-y-2"
                     x-transition:enter-end="opacity-100 translate-y-0">
                    <template x-for="comment in post.comments" :key="comment.text">
                        <div class="flex items-start space-x-2">
                            <span class="text-2xl" x-text="comment.avatar"></span>
                            <div class="flex-1 bg-gray-100 rounded-lg px-3 py-2">
                                <p class="font-semibold text-sm" x-text="comment.user"></p>
                                <p class="text-sm text-gray-700" x-text="comment.text"></p>
                            </div>
                        </div>
                    </template>

                    <!-- Add comment -->
                    <div class="flex items-center space-x-2 pt-2">
                        <span class="text-2xl" x-text="currentUser.avatar"></span>
                        <input type="text"
                               x-model="newComment[post.id]"
                               @keydown.enter="addComment(post)"
                               placeholder="Add a comment..."
                               class="flex-1 px-3 py-2 bg-gray-100 rounded-full comment-input text-sm">
                        <button @click="addComment(post)"
                                class="text-blue-500 hover:text-blue-600 font-semibold text-sm">
                            Post
                        </button>
                    </div>
                </div>
            </div>
        </template>
        """,
        "max-w-2xl mx-auto px-4"
    )

    # Right sidebar (desktop)
    right_sidebar = wv.div(
        wv.render(
            # Suggestions
            wv.div(
                wv.render(
                    wv.h3("Suggestions For You", "text-sm font-semibold text-gray-800 mb-4"),
                    wv.div(
                        wv.render(
                            # Suggestion items
                            wv.div(
                                wv.render(
                                    wv.div(
                                        wv.render(
                                            wv.div("👨‍🎨", "text-3xl"),
                                            wv.div(
                                                wv.render(
                                                    wv.p("Artist Pro", "font-semibold text-sm"),
                                                    wv.p("Followed by john +2", "text-xs text-gray-500")
                                                ),
                                                ""
                                            )
                                        ),
                                        "flex items-center space-x-3"
                                    ),
                                    wv.button("Follow", "text-blue-500 hover:text-blue-600 font-semibold text-sm")
                                ),
                                "flex items-center justify-between py-2"
                            ),
                            wv.div(
                                wv.render(
                                    wv.div(
                                        wv.render(
                                            wv.div("👩‍💻", "text-3xl"),
                                            wv.div(
                                                wv.render(
                                                    wv.p("CodeMaster", "font-semibold text-sm"),
                                                    wv.p("Popular", "text-xs text-gray-500")
                                                ),
                                                ""
                                            )
                                        ),
                                        "flex items-center space-x-3"
                                    ),
                                    wv.button("Follow", "text-blue-500 hover:text-blue-600 font-semibold text-sm")
                                ),
                                "flex items-center justify-between py-2"
                            ),
                            wv.div(
                                wv.render(
                                    wv.div(
                                        wv.render(
                                            wv.div("🎮", "text-3xl"),
                                            wv.div(
                                                wv.render(
                                                    wv.p("GamerZone", "font-semibold text-sm"),
                                                    wv.p("Followed by emma", "text-xs text-gray-500")
                                                ),
                                                ""
                                            )
                                        ),
                                        "flex items-center space-x-3"
                                    ),
                                    wv.button("Follow", "text-blue-500 hover:text-blue-600 font-semibold text-sm")
                                ),
                                "flex items-center justify-between py-2"
                            )
                        ),
                        ""
                    )
                ),
                "bg-white rounded-lg shadow-sm p-4 mb-4"
            ),
            # Footer
            wv.div(
                wv.render(
                    wv.p("© 2025 SocialHub", "text-xs text-gray-500 mb-2"),
                    wv.div(
                        wv.render(
                            wv.link("About", "#", "text-xs text-gray-500 hover:underline"),
                            wv.span("·", "text-xs text-gray-400 mx-1"),
                            wv.link("Help", "#", "text-xs text-gray-500 hover:underline"),
                            wv.span("·", "text-xs text-gray-400 mx-1"),
                            wv.link("Terms", "#", "text-xs text-gray-500 hover:underline")
                        ),
                        "flex"
                    )
                ),
                "px-4"
            )
        ),
        "desktop-sidebar hidden lg:block w-80 fixed right-4 top-20"
    )

    # Main layout
    main_layout = wv.div(
        wv.render(
            nav_bar,
            wv.div(
                wv.render(
                    stories_section,
                    posts_feed,
                    right_sidebar
                ),
                "container mx-auto max-w-6xl pt-4"
            )
        ),
        "", "", "", {"x-data": "socialFeed()"}
    )

    # Build complete HTML
    head_content = wv.render(
        wv.title("SocialHub - Social Media Feed"),
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
            wv.body(main_layout, "bg-gray-50 min-h-screen")
        )
    )

    return full_html

# Create and display the social feed
print("Creating social media feed with interactive posts...")
window = wv.Window("SocialHub - Social Media Feed", 1400, 900)
window.set_html(create_social_feed())
print("Social feed created! Launching window...")
window.run()
