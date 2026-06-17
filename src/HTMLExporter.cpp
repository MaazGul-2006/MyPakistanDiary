#include "HTMLExporter.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

HTMLExporter::HTMLExporter(Database& db, const std::string& outputPath)
    : m_db(db), m_outputPath(outputPath) {}

bool HTMLExporter::exportToHTML(const std::vector<TravelEntry>& entries) {
    std::ofstream file(m_outputPath);
    if (!file.is_open()) {
        std::cerr << "Cannot open file for writing: " << m_outputPath << "\n";
        return false;
    }

    file << generateHeader();

    for (const auto& entry : entries) {
        file << generateEntryCard(entry);
    }

    file << generateCrowdWisdom();
    file << generateFooter();

    file.close();
    std::cout << "HTML report generated: " << m_outputPath << "\n";
    return true;
}

std::string HTMLExporter::generateHeader() const {
    return R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>My Pakistan — Travel Diary</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #1a1a2e 0%, #16213e 100%);
            color: #f0f0f0;
            padding: 20px;
            min-height: 100vh;
        }

        .container {
            max-width: 900px;
            margin: 0 auto;
        }

        header {
            background: linear-gradient(135deg, #0a6b3f 0%, #00a86d 100%);
            padding: 40px 20px;
            border-radius: 12px;
            text-align: center;
            margin-bottom: 40px;
            box-shadow: 0 10px 30px rgba(0, 0, 0, 0.3);
        }

        header h1 {
            font-size: 2.5em;
            margin-bottom: 10px;
            text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.3);
        }

        header p {
            font-size: 1.1em;
            opacity: 0.9;
        }

        .entry {
            background: rgba(30, 30, 42, 0.8);
            border-left: 5px solid #0a6b3f;
            padding: 25px;
            margin-bottom: 20px;
            border-radius: 8px;
            backdrop-filter: blur(10px);
            box-shadow: 0 4px 15px rgba(0, 0, 0, 0.2);
            transition: transform 0.2s, box-shadow 0.2s;
        }

        .entry:hover {
            transform: translateY(-2px);
            box-shadow: 0 6px 20px rgba(0, 0, 0, 0.3);
        }

        .entry h2 {
            font-size: 1.8em;
            margin-bottom: 15px;
            color: #fff;
        }

        .entry-meta {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 15px;
            margin-bottom: 20px;
        }

        .meta-item {
            background: rgba(0, 168, 107, 0.1);
            padding: 10px 15px;
            border-radius: 6px;
            border-left: 3px solid #0a6b3f;
        }

        .meta-label {
            font-size: 0.85em;
            color: #a0a0b4;
            text-transform: uppercase;
            font-weight: 600;
            margin-bottom: 5px;
        }

        .meta-value {
            font-size: 1.1em;
            color: #fff;
        }

        .mood-badge {
            display: inline-block;
            padding: 5px 12px;
            border-radius: 20px;
            font-weight: 600;
            font-size: 0.9em;
            color: white;
        }

        .rating {
            font-size: 1.2em;
            color: #ffc800;
            letter-spacing: 2px;
        }

        .category {
            display: inline-block;
            background: rgba(100, 180, 255, 0.2);
            color: #64b4ff;
            padding: 4px 10px;
            border-radius: 4px;
            font-size: 0.9em;
            margin-right: 10px;
        }

        .status {
            display: inline-block;
            padding: 4px 10px;
            border-radius: 4px;
            font-size: 0.9em;
            font-weight: 600;
        }

        .status.visited {
            background: rgba(100, 220, 130, 0.2);
            color: #64dc82;
        }

        .status.planned {
            background: rgba(255, 180, 50, 0.2);
            color: #ffb432;
        }

        .description {
            background: rgba(50, 50, 70, 0.4);
            padding: 15px;
            border-radius: 6px;
            margin-top: 15px;
            line-height: 1.6;
            font-style: italic;
            border-left: 3px solid #64b4ff;
        }

        .crowd-section {
            background: rgba(10, 107, 63, 0.15);
            border-left: 5px solid #0a6b3f;
            padding: 25px;
            margin: 40px 0;
            border-radius: 8px;
        }

        .crowd-section h3 {
            font-size: 1.5em;
            margin-bottom: 20px;
            color: #64dc82;
        }

        .crowd-item {
            background: rgba(30, 30, 42, 0.6);
            padding: 12px 15px;
            margin-bottom: 10px;
            border-radius: 6px;
            border-left: 3px solid #0a6b3f;
        }

        .crowd-item strong {
            color: #64dc82;
        }

        footer {
            text-align: center;
            padding: 30px;
            color: #a0a0b4;
            border-top: 1px solid rgba(160, 160, 180, 0.2);
            margin-top: 40px;
        }

        footer p {
            margin-bottom: 10px;
        }

        footer a {
            color: #0a6b3f;
            text-decoration: none;
            font-weight: 600;
        }

        footer a:hover {
            text-decoration: underline;
        }

        @media (max-width: 600px) {
            header h1 {
                font-size: 1.8em;
            }

            .entry-meta {
                grid-template-columns: 1fr;
            }

            body {
                padding: 10px;
            }
        }
    </style>
</head>
<body>
    <div class="container">
)";
}

std::string HTMLExporter::generateEntryCard(const TravelEntry& entry) const {
    std::ostringstream oss;
    
    oss << R"(        <article class="entry">
            <h2>)" << entry.getPlace() << R"(</h2>
            
            <div class="entry-meta">
                <div class="meta-item">
                    <div class="meta-label">City</div>
                    <div class="meta-value">)" << entry.getCity() << R"(</div>
                </div>
                
                <div class="meta-item">
                    <div class="meta-label">Date</div>
                    <div class="meta-value">)" << entry.getDate() << R"(</div>
                </div>
                
                <div class="meta-item">
                    <div class="meta-label">Mood</div>
                    <div class="mood-badge" style="background-color: )" 
        << getColorForMood(entry.getMood()) << R"(;">)" 
        << entry.getMood() << R"(</div>
                </div>
                
                <div class="meta-item">
                    <div class="meta-label">Rating</div>
                    <div class="rating">)" << entry.getRatingStars() << R"(</div>
                </div>
            </div>
            
            <div style="margin-bottom: 15px;">
                <span class="category">)" << entry.getCategory() << R"(</span>
                <span class="status )" << (entry.getStatus() == "Visited" ? "visited" : "planned") 
        << R"(">)" << entry.getStatus() << R"(</span>
            </div>
            
            <div class="description">
                )" << entry.getDescription() << R"(
            </div>
        </article>
)";
    return oss.str();
}

std::string HTMLExporter::generateCrowdWisdom() const {
    std::ostringstream oss;

    oss << R"(        <section class="crowd-section">
            <h3>Crowd Wisdom — Community Insights</h3>
            
)";

    // Get cities and their stats
    std::vector<std::string> cities = {"Islamabad", "Rawalpindi", "Lahore", "Murree"};
    
    for (const auto& city : cities) {
        int count = m_db.getVisitCount(city);
        if (count > 0) {
            std::string mood = m_db.getMostCommonMood(city);
            oss << R"(            <div class="crowd-item">
                <strong>)" << city << "</strong> — "
                << count << " visit" << (count > 1 ? "s" : "") 
                << " | Most common mood: <strong>" << mood << "</strong>"
                << R"(
            </div>
)";
        }
    }

    oss << R"(        </section>
)";
    return oss.str();
}

std::string HTMLExporter::generateFooter() const {
    return R"(    </div>

    <footer>
        <p><strong>My Pakistan — Personal Travel Diary</strong></p>
        <p>A decentralized travel diary celebrating Pakistani journeys and emotions.</p>
        <p>
            <strong>Support:</strong> If you need to talk, 
            <strong>Umang helpline: 0317-4288665</strong>
        </p>
        <p style="margin-top: 20px; font-size: 0.9em; opacity: 0.7;">
            Generated by My Pakistan Travel Diary | 
            <a href="https://github.com/maaz-gul/MyPakistanDiary" target="_blank">GitHub</a>
        </p>
    </footer>
</body>
</html>
)";
}

std::string HTMLExporter::getColorForMood(const std::string& mood) const {
    if (mood == "Peaceful")   return "#4682b4";  // steel blue
    if (mood == "Joyful")     return "#ffa500";  // orange
    if (mood == "Reflective") return "#8a2be2";  // purple
    if (mood == "Nostalgic")  return "#a0522d";  // sienna
    if (mood == "Energized")  return "#32cd32";  // lime green
    if (mood == "Anxious")    return "#dc143c";  // crimson
    return "#646478";                            // gray
}

std::string HTMLExporter::getCategoryEmoji(const std::string& category) const {
    if (category == "Historical")  return "🏛️";
    if (category == "Religious")   return "🕌";
    if (category == "Nature")      return "🏔️";
    if (category == "Recreation")  return "🎡";
    return "📍";
}