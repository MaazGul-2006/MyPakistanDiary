# My Pakistan — Personal Travel Diary

> A decentralized community travel diary celebrating Pakistani journeys, emotions, and mental wellness.

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![Language](https://img.shields.io/badge/language-C%2B%2B17-blue)
![License](https://img.shields.io/badge/license-MIT-green)

---

## 🎯 Project Vision

**My Pakistan Travel Diary** is a desktop application that transforms travel documentation into a gateway for emotional connection and peer support. Built during a week-long intensive, this project demonstrates how shared experiences—especially travel—can help university students overcome isolation and stigma around mental health.

### The Problem
Many Pakistani university students struggle silently with depression and anxiety, unable to share their feelings. Social barriers, stigma, and lack of safe spaces prevent them from seeking support.

### Our Solution
We embed wellness check-ins within a travel diary—making emotional expression feel natural and safe. When peers share their journeys, the app aggregates their collective insights (Wisdom of Crowds), creating community validation without centralization.

---

## ✨ Key Features

### 📔 Personal Travel Diary
- **Add Entries**: Place, city, date, personal notes, category, mood, rating
- **Rich Metadata**: Mood tags (Peaceful, Joyful, Reflective, Nostalgic, Energized, Anxious)
- **Status Tracking**: Mark trips as Visited or Planned
- **Photo Support**: Link travel photos (JPG/PNG)

### 🛡️ Mental Wellness Layer
- **Pre-Trip Questionnaire**: When marking a trip as "Planned", users answer 4 wellness questions:
  - Are you traveling alone?
  - How are you feeling about this trip?
  - Does anyone know your travel plans?
  - What's your energy level today?
- **Discomfort Reporting**: Flag unsafe experiences at locations (future: crowd-alerts)
- **Support Resources**: If responses flag concern → display **Umang helpline: 0317-4288665**

### 🌍 Wisdom of Crowds
Aggregate community insights without a central server:
- **Average Ratings per Place**: See how others rated the same destination
- **Visit Frequency Heatmap**: Which Pakistani cities are most visited?
- **Mood Distribution**: "Most travelers feel Peaceful at Faisal Mosque"
- **Hidden Gem Score**: High ratings + low visit count = underrated treasure

### 📤 Decentralized Sharing
- **JSON Import/Export**: Share your diary with friends as `.json` files
- **No Server Required**: Each SQLite database is an independent node
- **HTML Export**: Beautiful styled report (see screenshots) — open in any browser
- **Community Aggregation**: Import friends' entries locally, crowd wisdom emerges

### 🎨 User Interface
- **SFML-based GUI**: Smooth, responsive desktop app
- **Dark Theme**: Eye-friendly dark mode with Pakistan green accents (#0A6B3F)
- **Keyboard Navigation**: Arrow keys, Tab, Enter — full terminal-style control
- **Entry Cards**: Color-coded by mood, with at-a-glance info

---

## 🏗️ Architecture

### Tech Stack
- **Language**: C++17 (OOP + PF concepts)
- **GUI**: SFML 2.5 (graphics, windowing)
- **Database**: SQLite3 (lightweight, local, decentralized)
- **Export**: HTML + CSS (styled reports)
- **Build**: CMake 3.16+
- **OS**: Linux (Ubuntu 20.04+)

### File Structure
MyPakistanDiary/

├── include/

│   ├── TravelEntry.h        # Core data model

│   ├── Database.h           # SQLite CRUD

│   ├── DiaryApp.h           # SFML window & UI controller

│   ├── PreTripCheck.h       # Wellness questionnaire

│   └── HTMLExporter.h       # Report generation

├── src/

│   ├── main.cpp             # Entry point

│   ├── TravelEntry.cpp

│   ├── Database.cpp

│   ├── DiaryApp.cpp

│   ├── PreTripCheck.cpp

│   └── HTMLExporter.cpp

├── database/

│   └── diary.db             # SQLite (auto-created)

├── assets/

│   ├── fonts/               # DejaVuSans.ttf

│   ├── icons/               # Future: mood sprites

│   └── photos/              # User travel photos

├── export/

│   └── travel_report.html   # Generated report

├── docs/

│   └── screenshots/         # Portfolio evidence

├── CMakeLists.txt

├── README.md

└── .gitignore

### Data Model

**TravelEntry**
```cpp
int id, float rating
string place, city, date, description, category
string mood (Peaceful|Joyful|Reflective|Nostalgic|Energized|Anxious)
string status (Visited|Planned)
string photoPath
bool isAnonymous, discomfortFlag
```

**PreTripCheck**
```cpp
bool travelingAlone
int feelingScore (1-4: Excited, Nervous, NeedBreak, Unsure)
bool sharedWithSomeone
int energyLevel (1-3: Low, Medium, High)
bool flagged (computed from above)
```

---

## 🚀 Quick Start

### Prerequisites
```bash
sudo apt update
sudo apt install libsfml-dev libsqlite3-dev cmake g++
```

### Build
```bash
cd MyPakistanDiary
mkdir -p build
cd build
cmake ..
make
```

### Run
```bash
./MyPakistanDiary
```

### Controls
| Key | Action |
|-----|--------|
| `↑` / `↓` | Scroll entries |
| `Enter` | View entry details / Confirm |
| `A` | Add new entry |
| `E` | Export to HTML |
| `Esc` | Back / Quit |
| `Tab` | Next field (in form) |
| `Backspace` | Delete character (in form) |

---

## 📖 Usage Example

### Add a Personal Memory
1. Press `A` to open form
2. Fill fields: "Murree", "Murree", "2025-07-10", "Summer escape..."
3. Set Status to `Visited`
4. Press `Enter` → Entry saved to database

### Add a Planned Trip with Wellness Check
1. Press `A`
2. Fill: "Hunza Valley", "Gilgit-Baltistan", "2025-09-01"
3. Set Status to `Planned`
4. Press `Enter`
5. Answer 4 wellness questions (Y, 2, N, 1)
6. If flagged → See support message with helpline
7. Entry saved; return to list

### Export & Share
1. Press `E` from list
2. HTML report generated to `export/travel_report.html`
3. Open in browser → Beautiful styled report with crowd wisdom
4. Share file with friends — they import it for aggregated insights

---

## 🎓 Learning Outcomes

This project demonstrates:
- **OOP Design**: Clean class hierarchy (TravelEntry, Database, DiaryApp)
- **Database Design**: SQLite schema, CRUD operations, aggregation queries
- **GUI Programming**: SFML event handling, rendering pipeline, state machines
- **C++ Proficiency**: Smart pointers, STL containers, file I/O, string manipulation
- **Software Architecture**: Decentralized design, separation of concerns, scalability
- **User-Centric Design**: Accessibility (keyboard-first), gentle UX for sensitive topic

---

## 🌱 Mental Health & Wellbeing

This app recognizes that **travel** is more than tourism—it's therapeutic. By embedding wellness check-ins, we:

- **Normalize Emotional Expression**: Travel memories naturally include feelings
- **Create Safe Spaces**: Anonymous sharing option included
- **Build Community**: Wisdom of Crowds shows "you're not alone"
- **Provide Resources**: Every flagged entry links to **Umang helpline: 0317-4288665**

### If You Need Support
- **Pakistan**: Umang (AASRA) — 0317-4288665
- **Internationally**: See [findahelpline.com](https://findahelpline.com)

---

## 📊 Sample Data

The app ships with 9 real Pakistani travel memories:
- Shah Allah Ditta Caves, Islamabad
- Lal Masjid, Islamabad
- Taxila Buddhist Site, Rawalpindi
- Army Museum, Rawalpindi
- Faisal Mosque, Islamabad
- Murree Mall Road & Chair Lift
- Lahore (Shahi Qila, Minar-e-Pakistan, Joyland)

All entries use real dates and authentic emotions.

---

## 🔮 Future Roadmap

### V2 — Multi-User Community
- Web version with backend sync
- Real multi-user entries (not just import/export)
- Discomfort alerts for regions
- Travel recommendation engine

### V3 — University Integration
- Deployed on campus networks
- Peer support features
- Integration with counseling services
- Anonymous journaling modes

### V4 — AI Features
- Sentiment analysis on descriptions
- Wellness score per region
- Personalized travel recommendations
- Mental health trend tracking

---

## 🛠️ Development Notes

### Why Decentralized?
- **Privacy**: No central server storing personal data
- **Resilience**: Works offline; sync when convenient
- **Trust**: Users control their own data
- **Scalability**: Each node (laptop/phone) acts independently

### Why SQLite?
- Zero configuration
- File-based (easy to backup/share)
- Rich query support for Wisdom of Crowds
- Lightweight for desktop

### Why SFML?
- Cross-platform
- Simple graphics API
- Keyboard-first (perfect for terminal-like apps)
- Lightweight for modern UIs

---

## 📸 Screenshots

See `docs/` folder for:
- List view with 9 entries
- Detail view with full notes
- Add entry form with validation
- Wellness questionnaire screens
- HTML export in browser
- Crowd Wisdom aggregation

---

## 🤝 Contributing

This is a portfolio project built in 7 days as proof-of-concept. Future contributors welcome!

### Code Style
- C++17 conventions
- Clear naming (no abbreviations except `m_` for members)
- Comments for complex logic
- Consistent indentation (4 spaces)

---

## 📄 License

MIT License — See LICENSE file (free to use, modify, share)

---

## 👨‍💻 Author

**Muhammad Maaz Gul**
- Student, Data Science, FAST-NUCES Islamabad
- GitHub: [@maaz-gul](https://github.com/maaz-gul)
- Portfolio: [Your Portfolio Link]

---

## 🙏 Acknowledgments

- FAST-NUCES for mentorship & infrastructure
- Inspired by *Wisdom of Crowds* (James Surowiecki)
- SFML & SQLite communities
- Mental health advocates in Pakistan

---

**Built with ❤️ for a healthier, more connected Pakistan.**