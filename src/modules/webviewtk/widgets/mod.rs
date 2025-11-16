// Widgets module - Flutter-style widgets

// Basic widgets
pub mod text;
pub mod container;
pub mod center;
pub mod padding;
pub mod sized_box;
pub mod custom_titlebar;

// Layout widgets  
pub mod column;
pub mod row;
pub mod stack;
pub mod expanded;
pub mod flexible;
pub mod spacer;
pub mod positioned;

// Material widgets
pub mod button;
pub mod textfield;
pub mod card;
pub mod scaffold;
pub mod appbar;
pub mod fab;
pub mod divider;
pub mod list_tile;

// Gesture widgets
pub mod gesture_detector;
pub mod ink_well;

// Re-exports
pub use text::Text;
pub use container::Container;
pub use center::Center;
pub use padding::Padding;
pub use sized_box::SizedBox;
pub use column::Column;
pub use row::Row;
// Templated widgets - structs not yet implemented
// pub use stack::Stack;
// pub use expanded::Expanded;
// pub use flexible::Flexible;
// pub use spacer::Spacer;
// pub use positioned::Positioned;
// pub use button::Button;
// pub use textfield::TextField;
// pub use card::Card;
// pub use scaffold::Scaffold;
// pub use appbar::AppBar;
// pub use fab::FloatingActionButton;
// pub use divider::Divider;
// pub use list_tile::ListTile;
// pub use gesture_detector::GestureDetector;
// pub use ink_well::InkWell;
