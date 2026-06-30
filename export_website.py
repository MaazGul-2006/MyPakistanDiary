#!/usr/bin/env python3
"""
My Pakistan Travel Diary - Professional HTML Website Generator
Generates multi-page tourism promotion website from SQLite database
Uses OpenStreetMap + Leaflet (completely free, no API key required)
"""

import sqlite3
import os
import shutil
from datetime import datetime

class TravelWebsiteGenerator:
    def __init__(self, db_path, output_dir="export"):
        self.db_path = db_path
        self.output_dir = output_dir
        self.conn = None
        
    def connect_db(self):
        """Connect to SQLite database"""
        self.conn = sqlite3.connect(self.db_path)
        self.conn.row_factory = sqlite3.Row
        return self.conn.cursor()
    
    def setup_output(self):
        """Create export directories"""
        os.makedirs(f"{self.output_dir}/css", exist_ok=True)
        os.makedirs(f"{self.output_dir}/photos", exist_ok=True)
        
    def get_cities(self):
        """Fetch all cities from database"""
        cursor = self.connect_db()
        cursor.execute("SELECT * FROM cities ORDER BY name")
        return cursor.fetchall()
    
    def get_entries_by_city(self, city_name):
        """Fetch entries for a specific city"""
        cursor = self.connect_db()
        cursor.execute("SELECT * FROM travel_entries WHERE city = ? ORDER BY date DESC", (city_name,))
        return cursor.fetchall()
    
    def get_all_entries(self):
        """Fetch all entries"""
        cursor = self.connect_db()
        cursor.execute("SELECT * FROM travel_entries")
        return cursor.fetchall()
    
    def generate_css_base(self):
        """Generate base shared CSS"""
        css = """
/* ========================================
   My Pakistan Travel Diary - Base Styles
   ======================================== */

* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
}

:root {
    --color-primary: #0a6b3f;
    --color-primary-light: #00a86d;
    --color-primary-dark: #055033;
    
    /* Light mode defaults */
    --bg-primary: #ffffff;
    --bg-secondary: #f8f9fa;
    --bg-tertiary: #e9ecef;
    --text-primary: #212529;
    --text-secondary: #495057;
    --border-color: #dee2e6;
    --card-shadow: rgba(0, 0, 0, 0.1);
    --card-hover-shadow: rgba(0, 0, 0, 0.15);
}

body.dark-mode {
    --bg-primary: #1a1a2e;
    --bg-secondary: #16213e;
    --bg-tertiary: #0f3460;
    --text-primary: #e0e0e0;
    --text-secondary: #b0b0b0;
    --border-color: #2d3561;
    --card-shadow: rgba(0, 0, 0, 0.3);
    --card-hover-shadow: rgba(0, 0, 0, 0.4);
}

html {
    scroll-behavior: smooth;
}

body {
    font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
    line-height: 1.6;
    background-color: var(--bg-primary);
    color: var(--text-primary);
    transition: background-color 0.3s ease, color 0.3s ease;
}

/* ========================================
   Navbar
   ======================================== */

.navbar {
    background: linear-gradient(135deg, var(--color-primary) 0%, var(--color-primary-light) 100%);
    padding: 1rem 2rem;
    position: sticky;
    top: 0;
    z-index: 1000;
    display: flex;
    justify-content: space-between;
    align-items: center;
    box-shadow: 0 2px 8px var(--card-shadow);
}

.navbar-brand {
    font-size: 1.5rem;
    font-weight: bold;
    color: white;
    text-decoration: none;
    display: flex;
    align-items: center;
    gap: 0.5rem;
    transition: opacity 0.3s;
}

.navbar-brand:hover {
    opacity: 0.9;
}

.navbar-nav {
    display: flex;
    gap: 2rem;
    align-items: center;
    flex-wrap: wrap;
}

.nav-link {
    color: white;
    text-decoration: none;
    font-size: 1rem;
    transition: opacity 0.3s;
    font-weight: 500;
}

.nav-link:hover {
    opacity: 0.8;
}

.theme-toggle {
    padding: 0.6rem 1.2rem;
    border-radius: 25px;
    border: 2px solid white;
    background: rgba(255, 255, 255, 0.2);
    color: white;
    cursor: pointer;
    font-size: 0.95rem;
    font-weight: 600;
    transition: all 0.3s;
}

.theme-toggle:hover {
    background: rgba(255, 255, 255, 0.3);
    transform: scale(1.05);
}

/* ========================================
   Hero Section
   ======================================== */

.hero {
    background: linear-gradient(135deg, rgba(10, 107, 63, 0.9) 0%, rgba(0, 168, 109, 0.9) 100%),
                url('data:image/svg+xml,<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1200 400"><path fill="rgba(255,255,255,0.1)" d="M0,200 Q300,100 600,200 T1200,200 L1200,400 L0,400 Z"/></svg>');
    background-size: cover;
    background-position: center;
    background-attachment: fixed;
    color: white;
    padding: 6rem 2rem;
    text-align: center;
    min-height: 400px;
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
}

.hero h1 {
    font-size: 3.5rem;
    margin-bottom: 1rem;
    text-shadow: 2px 2px 8px rgba(0, 0, 0, 0.4);
    animation: fadeInDown 0.8s ease;
}

.hero p {
    font-size: 1.3rem;
    margin-bottom: 2rem;
    text-shadow: 1px 1px 4px rgba(0, 0, 0, 0.4);
    animation: fadeInUp 0.8s ease 0.2s backwards;
}

@keyframes fadeInDown {
    from { opacity: 0; transform: translateY(-30px); }
    to { opacity: 1; transform: translateY(0); }
}

@keyframes fadeInUp {
    from { opacity: 0; transform: translateY(30px); }
    to { opacity: 1; transform: translateY(0); }
}

/* ========================================
   Container & Grid
   ======================================== */

.container {
    max-width: 1200px;
    margin: 0 auto;
    padding: 0 2rem;
}

.grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
    gap: 2rem;
    margin: 3rem 0;
}

/* ========================================
   Cards
   ======================================== */

.card {
    background: var(--bg-primary);
    border: 1px solid var(--border-color);
    border-radius: 12px;
    padding: 2rem;
    box-shadow: 0 2px 8px var(--card-shadow);
    transition: all 0.3s ease;
}

.card:hover {
    transform: translateY(-4px);
    box-shadow: 0 8px 16px var(--card-hover-shadow);
}

.card h3 {
    color: var(--color-primary-light);
    font-size: 1.5rem;
    margin-bottom: 1rem;
}

.card p {
    color: var(--text-secondary);
    line-height: 1.8;
}

.card a {
    color: var(--color-primary-light);
    text-decoration: none;
    font-weight: 600;
    display: inline-block;
    margin-top: 1rem;
    transition: opacity 0.3s;
}

.card a:hover {
    opacity: 0.8;
}

/* ========================================
   City Section
   ======================================== */

.city-section {
    margin: 4rem 0;
    padding: 3rem;
    background: var(--bg-secondary);
    border-radius: 12px;
    border-left: 5px solid var(--color-primary-light);
}

.city-header {
    margin-bottom: 2rem;
}

.city-header h2 {
    font-size: 2.5rem;
    color: var(--color-primary-light);
    margin-bottom: 0.5rem;
}

.city-description {
    font-size: 1.1rem;
    color: var(--text-secondary);
    line-height: 1.8;
}

/* ========================================
   Info Boxes
   ======================================== */

.city-info {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
    gap: 1.5rem;
    margin: 2rem 0;
}

.info-box {
    background: var(--bg-primary);
    padding: 1.5rem;
    border-radius: 8px;
    border-left: 4px solid var(--color-primary);
    box-shadow: 0 2px 8px var(--card-shadow);
    transition: transform 0.3s;
}

.info-box:hover {
    transform: translateY(-2px);
}

.info-box h4 {
    color: var(--color-primary-light);
    margin-bottom: 0.5rem;
    font-size: 1.1rem;
}

.info-box p {
    color: var(--text-secondary);
    line-height: 1.6;
}

/* ========================================
   Maps
   ======================================== */

.map-container, #map {
    width: 100%;
    height: 500px;
    border-radius: 8px;
    margin: 2rem 0;
    box-shadow: 0 4px 12px var(--card-shadow);
    border: 1px solid var(--border-color);
}

.leaflet-container {
    background: var(--bg-secondary) !important;
    border-radius: 8px;
}

/* ========================================
   Travel Entries
   ======================================== */

.entries-section h3 {
    font-size: 1.8rem;
    margin: 2rem 0 1rem;
    color: var(--text-primary);
}

.entry-card {
    background: var(--bg-primary);
    border: 1px solid var(--border-color);
    border-radius: 8px;
    padding: 1.5rem;
    margin-bottom: 1.5rem;
    box-shadow: 0 2px 8px var(--card-shadow);
    transition: all 0.3s;
}

.entry-card:hover {
    transform: translateY(-2px);
    box-shadow: 0 4px 12px var(--card-hover-shadow);
}

.entry-card h4 {
    font-size: 1.3rem;
    margin-bottom: 0.5rem;
    color: var(--text-primary);
}

.entry-meta {
    display: flex;
    gap: 1rem;
    margin: 1rem 0;
    flex-wrap: wrap;
}

.meta-tag {
    padding: 0.4rem 0.9rem;
    border-radius: 20px;
    font-size: 0.85rem;
    background: rgba(0, 168, 109, 0.15);
    color: var(--color-primary-light);
    font-weight: 500;
}

.mood-badge {
    padding: 0.4rem 0.9rem;
    border-radius: 20px;
    font-size: 0.85rem;
    font-weight: 600;
    color: white;
}

.entry-description {
    color: var(--text-secondary);
    line-height: 1.8;
    margin: 1rem 0;
    font-style: italic;
    font-size: 0.95rem;
}

.entry-photo {
    margin: 1.5rem 0;
    max-width: 100%;
    border-radius: 8px;
    box-shadow: 0 4px 12px var(--card-shadow);
    transition: transform 0.3s;
}

.entry-photo:hover {
    transform: scale(1.02);
}

/* ========================================
   Footer
   ======================================== */

footer {
    background: linear-gradient(135deg, rgba(10, 107, 63, 0.95) 0%, rgba(0, 168, 109, 0.95) 100%);
    color: white;
    padding: 3rem 2rem;
    text-align: center;
    margin-top: 4rem;
}

footer p {
    margin: 0.5rem 0;
}

footer a {
    color: #e0e0e0;
    text-decoration: none;
    transition: opacity 0.3s;
}

footer a:hover {
    opacity: 0.8;
}

/* ========================================
   Responsive Design
   ======================================== */

@media (max-width: 768px) {
    .navbar {
        flex-direction: column;
        gap: 1rem;
    }
    
    .navbar-nav {
        gap: 1rem;
    }
    
    .hero h1 {
        font-size: 2rem;
    }
    
    .hero p {
        font-size: 1rem;
    }
    
    .city-header h2 {
        font-size: 1.8rem;
    }
    
    .grid {
        grid-template-columns: 1fr;
    }
    
    .map-container, #map {
        height: 300px;
    }
    
    .entry-meta {
        gap: 0.5rem;
    }
}

@media (max-width: 480px) {
    .hero {
        padding: 3rem 1rem;
    }
    
    .hero h1 {
        font-size: 1.5rem;
    }
    
    .container {
        padding: 0 1rem;
    }
    
    .card {
        padding: 1rem;
    }
    
    .city-section {
        padding: 1.5rem;
    }
}
"""
        return css
    
    def generate_index_html(self, cities):
        """Generate home page"""
        city_cards = ""
        for city in cities:
            city_cards += f"""
            <div class="card">
                <h3>{city['name']}</h3>
                <p>{city['description'][:120]}...</p>
                <p><strong>📅 Best Time:</strong> {city['best_time']}</p>
                <a href="city-{city['name'].lower().replace(' ', '-')}.html">
                   Explore {city['name']} →
                </a>
            </div>
            """
        
        html = f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>My Pakistan Travel Diary - Explore Pakistan's Hidden Gems</title>
    <link rel="stylesheet" href="css/styles.css">
</head>
<body>
    <nav class="navbar">
        <a href="index.html" class="navbar-brand">🇵🇰 My Pakistan Travel Diary</a>
        <div class="navbar-nav">
            <a href="cities.html" class="nav-link">All Cities</a>
            <a href="#cities" class="nav-link">Explore</a>
            <button class="theme-toggle" onclick="toggleTheme()">🌙 Dark Mode</button>
        </div>
    </nav>

    <section class="hero">
        <h1>Discover Pakistan's Hidden Gems</h1>
        <p>A personal journey through Pakistan's most beautiful destinations</p>
    </section>

    <section class="container">
        <h2 style="text-align: center; margin: 3rem 0; color: var(--color-primary-light); font-size: 2rem;">
            Featured Destinations
        </h2>
        <div class="grid" id="cities">
            {city_cards}
        </div>
    </section>

    <footer>
        <p><strong>My Pakistan Travel Diary</strong></p>
        <p>🌍 Celebrating Pakistan's beauty, culture, and adventure</p>
        <p style="margin-top: 1rem; font-size: 0.9rem;">
            📸 Real memories • 🗺️ Travel guides • 💚 Promote tourism
        </p>
        <p style="margin-top: 1rem; color: rgba(255,255,255,0.9); font-size: 0.85rem;">
            Mental Wellness Support: Umang helpline <strong>0317-4288665</strong>
        </p>
    </footer>

    <script>
        function toggleTheme() {{
            const body = document.body;
            const button = document.querySelector('.theme-toggle');
            
            if (body.classList.contains('dark-mode')) {{
                body.classList.remove('dark-mode');
                button.textContent = '🌙 Dark Mode';
                localStorage.setItem('theme', 'light');
            }} else {{
                body.classList.add('dark-mode');
                button.textContent = '☀️ Light Mode';
                localStorage.setItem('theme', 'dark');
            }}
        }}
        
        // Load saved theme on page load
        if (localStorage.getItem('theme') === 'dark') {{
            document.body.classList.add('dark-mode');
            document.querySelector('.theme-toggle').textContent = '☀️ Light Mode';
        }}
    </script>
</body>
</html>
"""
        return html
    
    def generate_city_html(self, city, entries):
        """Generate individual city page"""
        
        # Mood color mapping
        mood_colors = {
            "Peaceful": "#4682B4",
            "Joyful": "#FFA500",
            "Reflective": "#8A2BE2",
            "Nostalgic": "#A0522D",
            "Energized": "#32CD32",
            "Anxious": "#DC143C"
        }
        
        # Build entries section
        entries_html = ""
        for entry in entries:
            mood = entry['mood'] or 'Peaceful'
            mood_color = mood_colors.get(mood, "#4682B4")
            
            photo_html = ""
            if entry['photo_path']:
                photo_html = f'<img src="photos/{os.path.basename(entry["photo_path"])}" alt="{entry["place"]}" class="entry-photo">'
            
            entries_html += f"""
            <div class="entry-card">
                <h4>{entry['place']}</h4>
                <div class="entry-meta">
                    <span class="meta-tag">📅 {entry['date']}</span>
                    <span class="mood-badge" style="background-color: {mood_color};">{mood}</span>
                    <span class="meta-tag">⭐ {entry['rating']}/5</span>
                    <span class="meta-tag">{entry['category']}</span>
                </div>
                <p class="entry-description">"{entry['description']}"</p>
                {photo_html}
            </div>
            """
        
        html = f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{city['name']} - My Pakistan Travel Diary</title>
    <link rel="stylesheet" href="css/styles.css">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/leaflet/1.9.4/leaflet.min.css" />
    <script src="https://cdnjs.cloudflare.com/ajax/libs/leaflet/1.9.4/leaflet.min.js"></script>
</head>
<body>
    <nav class="navbar">
        <a href="index.html" class="navbar-brand">🇵🇰 My Pakistan Travel Diary</a>
        <div class="navbar-nav">
            <a href="index.html" class="nav-link">Home</a>
            <a href="cities.html" class="nav-link">All Cities</a>
            <button class="theme-toggle" onclick="toggleTheme()">🌙 Dark Mode</button>
        </div>
    </nav>

    <section class="city-section">
        <div class="container">
            <div class="city-header">
                <h2>{city['name']}</h2>
                <p class="city-description">{city['description']}</p>
            </div>
            
            <div class="city-info">
                <div class="info-box">
                    <h4>📅 Best Time to Visit</h4>
                    <p>{city['best_time']}</p>
                </div>
                <div class="info-box">
                    <h4>🛡️ Safety Information</h4>
                    <p>{city['safety_info']}</p>
                </div>
                <div class="info-box">
                    <h4>✈️ Traveler Tips</h4>
                    <p>{city['travel_tips']}</p>
                </div>
            </div>
            
            <div class="map-container" id="map"></div>
            
            <div class="entries-section">
                <h3>My Travel Experiences in {city['name']}</h3>
                {entries_html if entries_html else '<p>No entries yet for this city.</p>'}
            </div>
        </div>
    </section>

    <footer>
        <p><strong>My Pakistan Travel Diary</strong></p>
        <p>Exploring {city['name']} - one journey at a time</p>
        <p style="margin-top: 0.5rem; font-size: 0.9rem;">
            📍 <strong>Coordinates:</strong> {city['latitude']:.4f}°N, {city['longitude']:.4f}°E
        </p>
    </footer>

    <script>
        // Initialize OpenStreetMap with Leaflet
        function initMap() {{
            const map = L.map('map').setView([{city['latitude']}, {city['longitude']}], 12);
            
            // OpenStreetMap tile layer (completely free)
            L.tileLayer('https://{{s}}.tile.openstreetmap.org/{{z}}/{{x}}/{{y}}.png', {{
                attribution: '&copy; OpenStreetMap contributors',
                maxZoom: 18,
                className: 'leaflet-tile'
            }}).addTo(map);
            
            // Add marker for the city
            L.marker([{city['latitude']}, {city['longitude']}], {{
                title: '{city['name']}'
            }}).addTo(map).bindPopup('<b>{city['name']}</b><br>Your travel destination').openPopup();
        }}
        
        // Initialize map when page loads
        window.addEventListener('load', initMap);
        
        function toggleTheme() {{
            const body = document.body;
            const button = document.querySelector('.theme-toggle');
            
            if (body.classList.contains('dark-mode')) {{
                body.classList.remove('dark-mode');
                button.textContent = '🌙 Dark Mode';
                localStorage.setItem('theme', 'light');
            }} else {{
                body.classList.add('dark-mode');
                button.textContent = '☀️ Light Mode';
                localStorage.setItem('theme', 'dark');
            }}
        }}
        
        // Load saved theme on page load
        if (localStorage.getItem('theme') === 'dark') {{
            document.body.classList.add('dark-mode');
            document.querySelector('.theme-toggle').textContent = '☀️ Light Mode';
        }}
    </script>
</body>
</html>
"""
        return html
    
    def generate_cities_overview(self, cities):
        """Generate cities overview page"""
        city_list = ""
        for city in cities:
            city_list += f"""
            <div class="card">
                <h3>{city['name']}</h3>
                <p><strong>📍 Location:</strong> {city['latitude']:.4f}°N, {city['longitude']:.4f}°E</p>
                <p>{city['description']}</p>
                <a href="city-{city['name'].lower().replace(' ', '-')}.html">
                   View Details & Map →
                </a>
            </div>
            """
        
        html = f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>All Destinations - My Pakistan Travel Diary</title>
    <link rel="stylesheet" href="css/styles.css">
</head>
<body>
    <nav class="navbar">
        <a href="index.html" class="navbar-brand">🇵🇰 My Pakistan Travel Diary</a>
        <div class="navbar-nav">
            <a href="index.html" class="nav-link">Home</a>
            <a href="cities.html" class="nav-link">All Cities</a>
            <button class="theme-toggle" onclick="toggleTheme()">🌙 Dark Mode</button>
        </div>
    </nav>

    <section class="hero">
        <h1>Explore All My Destinations</h1>
        <p>Discover every city in my Pakistan travel journey</p>
    </section>

    <section class="container">
        <div class="grid">
            {city_list}
        </div>
    </section>

    <footer>
        <p><strong>My Pakistan Travel Diary</strong></p>
        <p>Your guide to Pakistan's most beautiful and historic destinations</p>
        <p style="margin-top: 1rem; font-size: 0.85rem; color: rgba(255,255,255,0.9);">
            Built to promote Pakistan's tourism and showcase the beauty of each destination
        </p>
    </footer>

    <script>
        function toggleTheme() {{
            const body = document.body;
            const button = document.querySelector('.theme-toggle');
            
            if (body.classList.contains('dark-mode')) {{
                body.classList.remove('dark-mode');
                button.textContent = '🌙 Dark Mode';
                localStorage.setItem('theme', 'light');
            }} else {{
                body.classList.add('dark-mode');
                button.textContent = '☀️ Light Mode';
                localStorage.setItem('theme', 'dark');
            }}
        }}
        
        if (localStorage.getItem('theme') === 'dark') {{
            document.body.classList.add('dark-mode');
            document.querySelector('.theme-toggle').textContent = '☀️ Light Mode';
        }}
    </script>
</body>
</html>
"""
        return html
    
    def copy_photos(self):
        """Copy photos to export folder"""
        cursor = self.connect_db()
        cursor.execute("SELECT photo_path FROM travel_entries WHERE photo_path != ''")
        photos = cursor.fetchall()
        
        count = 0
        for photo_row in photos:
            photo_path = photo_row[0]
            if os.path.exists(photo_path):
                filename = os.path.basename(photo_path)
                dest = f"{self.output_dir}/photos/{filename}"
                try:
                    shutil.copy2(photo_path, dest)
                    count += 1
                except Exception as e:
                    print(f"⚠️  Could not copy {filename}: {e}")
        
        if count > 0:
            print(f"✓ Copied {count} photos")
    
    def generate(self):
        """Generate entire website"""
        print("\n" + "="*60)
        print("🌍 MY PAKISTAN TRAVEL DIARY - WEBSITE GENERATOR")
        print("="*60 + "\n")
        
        try:
            self.setup_output()
            
            # Get data from database
            cities = self.get_cities()
            
            if not cities:
                print("❌ No cities found in database!")
                print("   Please add cities first.\n")
                return False
            
            print(f"📍 Found {len(cities)} cities\n")
            
            # Generate CSS
            print("Generating styles...")
            with open(f"{self.output_dir}/css/styles.css", "w") as f:
                f.write(self.generate_css_base())
            print("  ✓ styles.css")
            
            # Generate HTML pages
            print("\nGenerating HTML pages...")
            with open(f"{self.output_dir}/index.html", "w") as f:
                f.write(self.generate_index_html(cities))
            print("  ✓ index.html (Home)")
            
            with open(f"{self.output_dir}/cities.html", "w") as f:
                f.write(self.generate_cities_overview(cities))
            print("  ✓ cities.html (All Destinations)")
            
            for city in cities:
                entries = self.get_entries_by_city(city['name'])
                filename = city['name'].lower().replace(' ', '-')
                with open(f"{self.output_dir}/city-{filename}.html", "w") as f:
                    f.write(self.generate_city_html(city, entries))
                entry_count = len(entries)
                print(f"  ✓ city-{filename}.html ({entry_count} entries)")
            
            # Copy photos
            print("\nCopying photos...")
            self.copy_photos()
            
            print("\n" + "="*60)
            print("✅ WEBSITE GENERATED SUCCESSFULLY!")
            print("="*60)
            print(f"\n📂 Open: {self.output_dir}/index.html")
            print("\n🎨 Features:")
            print("  • Light/Dark mode toggle")
            print("  • Free OpenStreetMap integration")
            print("  • Responsive mobile design")
            print("  • Professional tourism layout")
            print("  • City guides & travel tips")
            print("\n" + "="*60 + "\n")
            return True
            
        except Exception as e:
            print(f"❌ Error: {e}\n")
            return False

# Main execution
if __name__ == "__main__":
    # Use current directory database
    generator = TravelWebsiteGenerator("database/diary.db")
    success = generator.generate()
    exit(0 if success else 1)