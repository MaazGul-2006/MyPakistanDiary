#!/usr/bin/env python3
"""
My Pakistan Travel Diary - V3 Website Generator
Multi-page tourism promotion website with videos & media coverage
"""

import sqlite3
import os
import shutil

class TravelWebsiteGenerator:
    def __init__(self, db_path, output_dir="export"):
        self.db_path = db_path
        self.output_dir = output_dir
        self.conn = None
        
    def connect_db(self):
        self.conn = sqlite3.connect(self.db_path)
        self.conn.row_factory = sqlite3.Row
        return self.conn.cursor()
    
    def setup_output(self):
        os.makedirs(f"{self.output_dir}/css", exist_ok=True)
        os.makedirs(f"{self.output_dir}/photos", exist_ok=True)
    
    def get_cities(self):
        cursor = self.connect_db()
        cursor.execute("SELECT * FROM cities ORDER BY name")
        return cursor.fetchall()
    
    def get_entries_by_city(self, city_name):
        cursor = self.connect_db()
        cursor.execute("SELECT * FROM travel_entries WHERE city = ? ORDER BY date DESC", (city_name,))
        return cursor.fetchall()
    
    def get_videos_by_city(self, city_name):
        """Get all videos for a city"""
        cursor = self.connect_db()
        cursor.execute("SELECT youtube_url, title, description FROM videos WHERE city = ?", (city_name,))
        return cursor.fetchall()
    
    def get_media_by_city(self, city_name):
        """Get all media coverage for a city"""
        cursor = self.connect_db()
        cursor.execute("SELECT title, url, source_type, source_name FROM media_coverage WHERE city = ? ORDER BY source_type", (city_name,))
        return cursor.fetchall()
    
    def generate_css_base(self):
        """Base CSS (same as before)"""
        return """
/* ========================================
   My Pakistan Travel Diary V3 - Base Styles
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

/* Navbar */
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
}

.navbar-nav {
    display: flex;
    gap: 2rem;
    align-items: center;
}

.nav-link {
    color: white;
    text-decoration: none;
    font-size: 1rem;
    transition: opacity 0.3s;
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
    font-weight: 600;
    transition: all 0.3s;
}

.theme-toggle:hover {
    background: rgba(255, 255, 255, 0.3);
}

/* Hero */
.hero {
    background: linear-gradient(135deg, rgba(10, 107, 63, 0.9) 0%, rgba(0, 168, 109, 0.9) 100%);
    background-attachment: fixed;
    color: white;
    padding: 6rem 2rem;
    text-align: center;
    min-height: 400px;
    display: flex;
    flex-direction: column;
    justify-content: center;
}

.hero h1 {
    font-size: 3.5rem;
    margin-bottom: 1rem;
    text-shadow: 2px 2px 8px rgba(0, 0, 0, 0.4);
}

.hero p {
    font-size: 1.3rem;
    text-shadow: 1px 1px 4px rgba(0, 0, 0, 0.4);
}

/* Container */
.container {
    max-width: 1200px;
    margin: 0 auto;
    padding: 0 2rem;
}

/* Grid */
.grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
    gap: 2rem;
    margin: 3rem 0;
}

/* Card */
.card {
    background: var(--bg-primary);
    border: 1px solid var(--border-color);
    border-radius: 12px;
    padding: 2rem;
    box-shadow: 0 2px 8px var(--card-shadow);
    transition: all 0.3s;
}

.card:hover {
    transform: translateY(-4px);
    box-shadow: 0 8px 16px var(--card-hover-shadow);
}

.card h3 {
    color: var(--color-primary-light);
    margin-bottom: 1rem;
}

/* City Section */
.city-section {
    margin: 4rem 0;
    padding: 3rem;
    background: var(--bg-secondary);
    border-radius: 12px;
    border-left: 5px solid var(--color-primary-light);
}

.city-header h2 {
    font-size: 2.5rem;
    color: var(--color-primary-light);
    margin-bottom: 1rem;
}

.city-description {
    font-size: 1.1rem;
    color: var(--text-secondary);
}

/* Info Boxes */
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
}

.info-box h4 {
    color: var(--color-primary-light);
    margin-bottom: 0.5rem;
}

/* Videos Section */
.videos-section {
    margin: 3rem 0;
    padding: 2rem;
    background: var(--bg-primary);
    border-radius: 8px;
    border: 2px solid var(--color-primary-light);
}

.videos-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
    gap: 2rem;
    margin-top: 2rem;
}

.video-card {
    border-radius: 8px;
    overflow: hidden;
    box-shadow: 0 4px 12px var(--card-shadow);
    transition: transform 0.3s;
}

.video-card:hover {
    transform: translateY(-4px);
}

.video-card iframe {
    width: 100%;
    height: 250px;
    border: none;
}

.video-title {
    padding: 1rem;
    background: var(--bg-secondary);
    font-weight: 600;
    color: var(--text-primary);
}

/* Media Section */
.media-section {
    margin: 3rem 0;
    padding: 2rem;
    background: var(--bg-secondary);
    border-radius: 8px;
}

.media-by-type {
    margin: 1.5rem 0;
}

.media-type-title {
    font-size: 1.2rem;
    color: var(--color-primary-light);
    margin-bottom: 1rem;
    font-weight: 600;
}

.media-list {
    list-style: none;
    padding: 0;
}

.media-item {
    padding: 1rem;
    margin-bottom: 1rem;
    background: var(--bg-primary);
    border-left: 4px solid var(--color-primary);
    border-radius: 4px;
    box-shadow: 0 2px 8px var(--card-shadow);
    transition: all 0.3s;
}

.media-item:hover {
    transform: translateX(4px);
}

.media-item a {
    color: var(--color-primary-light);
    text-decoration: none;
    font-weight: 600;
    font-size: 1.05rem;
}

.media-item a:hover {
    text-decoration: underline;
}

.media-source {
    margin-top: 0.5rem;
    color: var(--text-secondary);
    font-size: 0.9rem;
}

/* Entries */
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
}

.entry-card h4 {
    font-size: 1.3rem;
    margin-bottom: 0.5rem;
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
}

.mood-badge {
    padding: 0.4rem 0.9rem;
    border-radius: 20px;
    font-size: 0.85rem;
    font-weight: 600;
    color: white;
}

.entry-photo {
    max-width: 100%;
    border-radius: 8px;
    margin: 1rem 0;
    box-shadow: 0 4px 12px var(--card-shadow);
}

/* Map */
#map, .map-container {
    width: 100%;
    height: 500px;
    border-radius: 8px;
    margin: 2rem 0;
    box-shadow: 0 4px 12px var(--card-shadow);
}

/* Footer */
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
}

footer a:hover {
    opacity: 0.8;
}

/* Responsive */
@media (max-width: 768px) {
    .navbar {
        flex-direction: column;
        gap: 1rem;
    }
    
    .hero h1 {
        font-size: 2rem;
    }
    
    .grid {
        grid-template-columns: 1fr;
    }
    
    #map, .map-container {
        height: 300px;
    }
    
    .videos-grid {
        grid-template-columns: 1fr;
    }
}
"""
    
    def generate_videos_section(self, videos):
        """Generate embedded YouTube videos section"""
        if not videos:
            return ""
        
        html = """
    <section class="videos-section">
        <h3 style="font-size: 1.8rem; color: var(--color-primary-light); margin-bottom: 1rem;">
            🎬 Destination Videos
        </h3>
        <p style="color: var(--text-secondary); margin-bottom: 2rem;">
            Watch travel vlogs and documentaries about this beautiful destination.
        </p>
        <div class="videos-grid">
"""
        
        for video in videos:
            youtube_id = video[0].split('embed/')[-1] if 'embed/' in video[0] else video[0]
            title = video[1] or "Travel Video"
            
            html += f"""
            <div class="video-card">
                <iframe src="https://www.youtube.com/embed/{youtube_id}" 
                    title="{title}"
                    allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
                    allowfullscreen>
                </iframe>
                <div class="video-title">{title}</div>
            </div>
"""
        
        html += """
        </div>
    </section>
"""
        return html
    
    def generate_media_section(self, media):
        """Generate media coverage links section"""
        if not media:
            return ""
        
        # Group by source type
        media_by_type = {}
        type_icons = {
            "Magazine": "📖",
            "News": "📡",
            "Vlogger": "🎥"
        }
        
        for item in media:
            source_type = item[2] or "Other"
            if source_type not in media_by_type:
                media_by_type[source_type] = []
            media_by_type[source_type].append(item)
        
        html = """
    <section class="media-section">
        <h3 style="font-size: 1.8rem; color: var(--color-primary-light); margin-bottom: 1rem;">
            📰 Media Coverage & Reviews
        </h3>
        <p style="color: var(--text-secondary); margin-bottom: 2rem;">
            See what travel magazines, news outlets, and vloggers are saying about this destination.
        </p>
"""
        
        for source_type in sorted(media_by_type.keys()):
            icon = type_icons.get(source_type, "📄")
            items = media_by_type[source_type]
            
            html += f"""
        <div class="media-by-type">
            <h4 class="media-type-title">{icon} {source_type}</h4>
            <ul class="media-list">
"""
            
            for item in items:
                title, url, _, source_name = item
                html += f"""
                <li class="media-item">
                    <a href="{url}" target="_blank">{title} →</a>
                    <p class="media-source">Source: {source_name or source_type}</p>
                </li>
"""
            
            html += """
            </ul>
        </div>
"""
        
        html += """
    </section>
"""
        return html
    
    def generate_city_html(self, city, entries):
        """Generate city page with videos and media"""
        
        videos = self.get_videos_by_city(city['name'])
        media = self.get_media_by_city(city['name'])
        videos_html = self.generate_videos_section(videos)
        media_html = self.generate_media_section(media)
        
        mood_colors = {
            "Peaceful": "#4682B4",
            "Joyful": "#FFA500",
            "Reflective": "#8A2BE2",
            "Nostalgic": "#A0522D",
            "Energized": "#32CD32",
            "Anxious": "#DC143C"
        }
        
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
                <p style="color: var(--text-secondary); font-style: italic; margin: 1rem 0;">"{entry['description']}"</p>
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
            
            {videos_html}
            
            {media_html}
            
            <div class="entries-section">
                <h3>My Travel Experiences in {city['name']}</h3>
                {entries_html if entries_html else '<p>No entries yet for this city.</p>'}
            </div>
        </div>
    </section>

    <footer>
        <p><strong>My Pakistan Travel Diary</strong></p>
        <p>Celebrating Pakistan's beauty, culture, and adventure</p>
        <p style="margin-top: 1rem; font-size: 0.9rem;">
            📍 {city['name']} • Coordinates: {city['latitude']:.4f}°N, {city['longitude']:.4f}°E
        </p>
        <p style="margin-top: 1rem; color: rgba(255,255,255,0.8); font-size: 0.85rem;">
            Mental Wellness Support: Umang helpline <strong>0317-4288665</strong>
        </p>
    </footer>

    <script src="https://maps.googleapis.com/maps/api/js?key="></script>
    <script>
        function initMap() {{
            const map = L.map('map').setView([{city['latitude']}, {city['longitude']}], 12);
            L.tileLayer('https://{{s}}.tile.openstreetmap.org/{{z}}/{{x}}/{{y}}.png', {{
                attribution: '&copy; OpenStreetMap contributors',
                maxZoom: 18
            }}).addTo(map);
            
            L.marker([{city['latitude']}, {city['longitude']}]).addTo(map)
                .bindPopup('<b>{city['name']}</b><br>{city['description'][:50]}...')
                .openPopup();
        }}
        
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
        
        if (localStorage.getItem('theme') === 'dark') {{
            document.body.classList.add('dark-mode');
            document.querySelector('.theme-toggle').textContent = '☀️ Light Mode';
        }}
    </script>
</body>
</html>
"""
        return html
    
    def generate_index_html(self, cities):
        """Generate home page"""
        city_cards = ""
        for city in cities:
            city_cards += f"""
            <div class="card">
                <h3>{city['name']}</h3>
                <p>{city['description'][:120]}...</p>
                <p><strong>📅 Best Time:</strong> {city['best_time']}</p>
                <a href="city-{city['name'].lower().replace(' ', '-')}.html" style="color: var(--color-primary-light); text-decoration: none; font-weight: 600;">
                   Explore {city['name']} →
                </a>
            </div>
            """
        
        html = f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>My Pakistan Travel Diary - Explore Hidden Gems</title>
    <link rel="stylesheet" href="css/styles.css">
</head>
<body>
    <nav class="navbar">
        <a href="index.html" class="navbar-brand">🇵🇰 My Pakistan Travel Diary</a>
        <div class="navbar-nav">
            <a href="cities.html" class="nav-link">All Cities</a>
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
        <div class="grid">
            {city_cards}
        </div>
    </section>

    <footer>
        <p><strong>My Pakistan Travel Diary</strong></p>
        <p>🌍 Celebrating Pakistan's beauty, culture, and adventure</p>
        <p style="margin-top: 1rem; font-size: 0.9rem;">
            📸 Real memories • 🎬 Destination videos • 📰 Media coverage • 🗺️ Travel guides
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
        """Generate all cities page"""
        city_list = ""
        for city in cities:
            city_list += f"""
            <div class="card">
                <h3>{city['name']}</h3>
                <p><strong>📍 Location:</strong> {city['latitude']:.4f}°N, {city['longitude']:.4f}°E</p>
                <p>{city['description']}</p>
                <a href="city-{city['name'].lower().replace(' ', '-')}.html" style="color: var(--color-primary-light); text-decoration: none; font-weight: 600; display: inline-block; margin-top: 1rem;">
                   View Details, Videos & Media →
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
            <button class="theme-toggle" onclick="toggleTheme()">🌙 Dark Mode</button>
        </div>
    </nav>

    <section class="hero">
        <h1>Explore All Destinations</h1>
        <p>Complete guide to every city in my Pakistan travel journey</p>
    </section>

    <section class="container">
        <div class="grid">
            {city_list}
        </div>
    </section>

    <footer>
        <p><strong>My Pakistan Travel Diary</strong></p>
        <p>Your gateway to Pakistan's most beautiful destinations</p>
        <p style="margin-top: 1rem; font-size: 0.85rem; color: rgba(255,255,255,0.9);">
            With destination videos, media coverage, travel tips, and real traveler experiences
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
        print("🌍 MY PAKISTAN TRAVEL DIARY - V3 WEBSITE GENERATOR")
        print("="*60 + "\n")
        
        try:
            self.setup_output()
            
            cities = self.get_cities()
            
            if not cities:
                print("❌ No cities found in database!")
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
                videos = self.get_videos_by_city(city['name'])
                media = self.get_media_by_city(city['name'])
                
                filename = city['name'].lower().replace(' ', '-')
                with open(f"{self.output_dir}/city-{filename}.html", "w") as f:
                    f.write(self.generate_city_html(city, entries))
                
                print(f"  ✓ city-{filename}.html ({len(entries)} entries, {len(videos)} videos, {len(media)} media)")
            
            # Copy photos
            print("\nCopying photos...")
            self.copy_photos()
            
            print("\n" + "="*60)
            print("✅ V3 WEBSITE GENERATED SUCCESSFULLY!")
            print("="*60)
            print(f"\n📂 Open: {self.output_dir}/index.html")
            print("\n🎨 Features:")
            print("  ✓ Light/Dark mode toggle")
            print("  ✓ YouTube embedded videos")
            print("  ✓ Media coverage links (Magazines, News, Vloggers)")
            print("  ✓ Free OpenStreetMap")
            print("  ✓ Responsive design")
            print("  ✓ Professional tourism layout")
            print("\n" + "="*60 + "\n")
            return True
            
        except Exception as e:
            print(f"❌ Error: {e}\n")
            import traceback
            traceback.print_exc()
            return False

if __name__ == "__main__":
    generator = TravelWebsiteGenerator("database/diary.db")
    success = generator.generate()
    exit(0 if success else 1)