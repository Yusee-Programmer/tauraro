// CDN Management for WebViewTK
// Provides constants and utilities for popular CSS/JS frameworks

use std::collections::HashMap;

/// Popular CDN constants
pub struct CDN;

impl CDN {
    // CSS Frameworks
    pub const TAILWIND_CSS: &'static str = "https://cdn.tailwindcss.com";  // Tailwind Play CDN (includes JIT compiler)
    pub const BOOTSTRAP_CSS: &'static str = "https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css";
    pub const BULMA_CSS: &'static str = "https://cdn.jsdelivr.net/npm/bulma@0.9.4/css/bulma.min.css";
    pub const FOUNDATION_CSS: &'static str = "https://cdn.jsdelivr.net/npm/foundation-sites@6.8.1/dist/css/foundation.min.css";
    pub const MATERIAL_CSS: &'static str = "https://cdn.jsdelivr.net/npm/@material/web@1.0.0/dist/material-web.min.css";
    pub const SEMANTIC_UI_CSS: &'static str = "https://cdn.jsdelivr.net/npm/semantic-ui@2.5.0/dist/semantic.min.css";
    
    // JavaScript Frameworks
    pub const ALPINE_JS: &'static str = "https://cdn.jsdelivr.net/npm/alpinejs@3.13.3/dist/cdn.min.js";
    pub const HTMX: &'static str = "https://cdn.jsdelivr.net/npm/htmx.org@1.9.10/dist/htmx.min.js";
    pub const VUE_JS: &'static str = "https://cdn.jsdelivr.net/npm/vue@3.4.15/dist/vue.global.prod.js";
    pub const REACT_JS: &'static str = "https://cdn.jsdelivr.net/npm/react@18.2.0/umd/react.production.min.js";
    pub const JQUERY: &'static str = "https://cdn.jsdelivr.net/npm/jquery@3.7.1/dist/jquery.min.js";
    
    // Bootstrap JavaScript
    pub const BOOTSTRAP_JS: &'static str = "https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/js/bootstrap.bundle.min.js";
    
    // Icon Libraries
    pub const FONT_AWESOME: &'static str = "https://cdn.jsdelivr.net/npm/@fortawesome/fontawesome-free@6.5.1/css/all.min.css";
    pub const MATERIAL_ICONS: &'static str = "https://fonts.googleapis.com/icon?family=Material+Icons";
    pub const BOOTSTRAP_ICONS: &'static str = "https://cdn.jsdelivr.net/npm/bootstrap-icons@1.11.3/font/bootstrap-icons.min.css";
    
    // Fonts
    pub const GOOGLE_FONTS_INTER: &'static str = "https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700&display=swap";
    pub const GOOGLE_FONTS_ROBOTO: &'static str = "https://fonts.googleapis.com/css2?family=Roboto:wght@400;500;700&display=swap";
    pub const GOOGLE_FONTS_POPPINS: &'static str = "https://fonts.googleapis.com/css2?family=Poppins:wght@400;500;600;700&display=swap";
    
    // Animation Libraries
    pub const ANIMATE_CSS: &'static str = "https://cdn.jsdelivr.net/npm/animate.css@4.1.1/animate.min.css";
    pub const GSAP: &'static str = "https://cdn.jsdelivr.net/npm/gsap@3.12.5/dist/gsap.min.js";
    
    // Utility Libraries
    pub const LODASH: &'static str = "https://cdn.jsdelivr.net/npm/lodash@4.17.21/lodash.min.js";
    pub const MOMENT_JS: &'static str = "https://cdn.jsdelivr.net/npm/moment@2.30.1/moment.min.js";
    pub const DAY_JS: &'static str = "https://cdn.jsdelivr.net/npm/dayjs@1.11.10/dayjs.min.js";
    pub const AXIOS: &'static str = "https://cdn.jsdelivr.net/npm/axios@1.6.5/dist/axios.min.js";
    
    // Chart Libraries
    pub const CHART_JS: &'static str = "https://cdn.jsdelivr.net/npm/chart.js@4.4.1/dist/chart.umd.min.js";
    pub const APEXCHARTS: &'static str = "https://cdn.jsdelivr.net/npm/apexcharts@3.45.2/dist/apexcharts.min.js";
    
    /// Get all available CDN constants as a map
    pub fn all_cdns() -> HashMap<String, String> {
        let mut cdns = HashMap::new();
        
        // CSS Frameworks
        cdns.insert("TAILWIND_CSS".to_string(), Self::TAILWIND_CSS.to_string());
        cdns.insert("BOOTSTRAP_CSS".to_string(), Self::BOOTSTRAP_CSS.to_string());
        cdns.insert("BULMA_CSS".to_string(), Self::BULMA_CSS.to_string());
        cdns.insert("FOUNDATION_CSS".to_string(), Self::FOUNDATION_CSS.to_string());
        cdns.insert("MATERIAL_CSS".to_string(), Self::MATERIAL_CSS.to_string());
        cdns.insert("SEMANTIC_UI_CSS".to_string(), Self::SEMANTIC_UI_CSS.to_string());
        
        // JavaScript
        cdns.insert("ALPINE_JS".to_string(), Self::ALPINE_JS.to_string());
        cdns.insert("HTMX".to_string(), Self::HTMX.to_string());
        cdns.insert("VUE_JS".to_string(), Self::VUE_JS.to_string());
        cdns.insert("REACT_JS".to_string(), Self::REACT_JS.to_string());
        cdns.insert("JQUERY".to_string(), Self::JQUERY.to_string());
        cdns.insert("BOOTSTRAP_JS".to_string(), Self::BOOTSTRAP_JS.to_string());
        
        // Icons
        cdns.insert("FONT_AWESOME".to_string(), Self::FONT_AWESOME.to_string());
        cdns.insert("MATERIAL_ICONS".to_string(), Self::MATERIAL_ICONS.to_string());
        cdns.insert("BOOTSTRAP_ICONS".to_string(), Self::BOOTSTRAP_ICONS.to_string());
        
        // Fonts
        cdns.insert("GOOGLE_FONTS_INTER".to_string(), Self::GOOGLE_FONTS_INTER.to_string());
        cdns.insert("GOOGLE_FONTS_ROBOTO".to_string(), Self::GOOGLE_FONTS_ROBOTO.to_string());
        cdns.insert("GOOGLE_FONTS_POPPINS".to_string(), Self::GOOGLE_FONTS_POPPINS.to_string());
        
        // Animation
        cdns.insert("ANIMATE_CSS".to_string(), Self::ANIMATE_CSS.to_string());
        cdns.insert("GSAP".to_string(), Self::GSAP.to_string());
        
        // Utilities
        cdns.insert("LODASH".to_string(), Self::LODASH.to_string());
        cdns.insert("MOMENT_JS".to_string(), Self::MOMENT_JS.to_string());
        cdns.insert("DAY_JS".to_string(), Self::DAY_JS.to_string());
        cdns.insert("AXIOS".to_string(), Self::AXIOS.to_string());
        
        // Charts
        cdns.insert("CHART_JS".to_string(), Self::CHART_JS.to_string());
        cdns.insert("APEXCHARTS".to_string(), Self::APEXCHARTS.to_string());
        
        cdns
    }
}

/// Generate HTML link/script tags for CDN resources
pub fn generate_cdn_tags(cdns: &[&str]) -> String {
    let mut tags = String::new();
    
    for cdn in cdns {
        if cdn.ends_with(".css") {
            tags.push_str(&format!("    <link rel=\"stylesheet\" href=\"{}\">\n", cdn));
        } else if cdn.ends_with(".js") {
            tags.push_str(&format!("    <script src=\"{}\" defer></script>\n", cdn));
        }
    }
    
    tags
}

/// Generate custom CSS style tag
pub fn generate_style_tag(css_content: &str) -> String {
    format!("    <style>\n{}\n    </style>\n", css_content)
}

/// Generate custom JavaScript script tag
pub fn generate_script_tag(js_content: &str) -> String {
    format!("    <script>\n{}\n    </script>\n", js_content)
}
