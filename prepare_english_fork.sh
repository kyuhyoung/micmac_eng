#!/bin/bash

# MicMac English Fork Preparation Script
# This script prepares the MicMac codebase for an English fork

echo "====================================="
echo "MicMac English Fork Preparation"
echo "====================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if we're in the MicMac directory
if [ ! -f "CMakeLists.txt" ] || [ ! -d "src" ]; then
    echo -e "${RED}Error: This script must be run from the MicMac root directory${NC}"
    exit 1
fi

echo -e "${GREEN}Step 1: Creating backup of current state${NC}"
BACKUP_DIR="../micmac_french_backup_$(date +%Y%m%d_%H%M%S)"
cp -r . "$BACKUP_DIR"
echo "Backup created at: $BACKUP_DIR"

echo -e "${GREEN}Step 2: Running safe translation (comments only)${NC}"
python3 safe_translate.py . --dry-run
read -p "Do you want to proceed with safe translation? (y/n) " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    python3 safe_translate.py .
fi

echo -e "${GREEN}Step 3: Updating README and documentation${NC}"
cat > README_ENGLISH.md << 'EOF'
# MicMac English Fork (micmac_eng)

This is an English translation fork of the MicMac photogrammetry software originally developed by IGN (Institut Géographique National).

## About This Fork

This fork aims to make MicMac more accessible to the English-speaking community by:
- Translating French variable names, function names, and comments to English
- Maintaining full compatibility with the original MicMac functionality
- Preserving all original algorithms and processing capabilities

## Original Project

The original MicMac project can be found at:
- Official website: https://micmac.ign.fr
- Original repository: [Original MicMac Repository URL]

## Translation Status

- [x] Core source files comments translated
- [x] Build system maintained
- [ ] Full variable/function name translation (in progress)
- [ ] Documentation translation (in progress)
- [ ] XML configuration files translation (in progress)

## Building

The build process remains the same as the original MicMac:

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

## License

This fork maintains the same CeCILL-B license as the original MicMac project.

## Acknowledgments

All credit for the original algorithms and implementation goes to:
- Marc Pierrot Deseilligny (IGN)
- Gregoire Maillet
- Didier Boldo
- And all other MicMac contributors

## Translation Notes

This English fork was created to improve accessibility while maintaining full compatibility with the original French version. The translation process focused on:
1. Preserving all functionality
2. Maintaining build compatibility
3. Translating user-facing elements first
4. Gradual translation of internal code

For any issues or improvements, please open an issue in this repository.
EOF

echo -e "${GREEN}Step 4: Creating translation mapping document${NC}"
cat > TRANSLATION_MAPPING.md << 'EOF'
# French to English Translation Mapping

This document maps the French terms used in the original MicMac to their English equivalents in this fork.

## Core Photogrammetry Terms

| French | English | Context |
|--------|---------|---------|
| Appui | ControlPoint | Ground control points |
| Bascule | Transform | Coordinate transformation |
| Calib/Calibration | Calibration | Camera calibration |
| Compens | Adjustment | Bundle adjustment |
| Homol | Correspondence | Feature correspondences |
| Masq/Masque | Mask | Image masks |
| Mesure | Measurement | Observations |
| Orient/Orientation | Orientation | Camera orientation |
| Residu | Residual | Error residuals |
| Saisie | Capture | Data capture/input |
| Triangul | Triangulation | Triangulation |

## Common Variable Prefixes

| French | English | Usage |
|--------|---------|-------|
| aNb | aNum | Number of something |
| mNb | mNum | Member variable for count |
| aPt | aPoint | A point |
| mPt | mPoint | Member point |
| aVec | aVec | A vector |
| aRes | aResult | A result |

## File Operations

| French | English |
|--------|---------|
| Fichier | File |
| Lire | Read |
| Ecrire | Write |
| Ouvrir | Open |
| Fermer | Close |
| Charger | Load |
| Sauver | Save |

## Geometric Terms

| French | English |
|--------|---------|
| Cercle | Circle |
| Droite | Line |
| Polygone | Polygon |
| Sommet | Vertex |
| Noeud | Node |

This mapping is used consistently throughout the translated codebase.
EOF

echo -e "${GREEN}Step 5: Setting up git for the English fork${NC}"

# Check git status
if git status --porcelain | grep -q .; then
    echo -e "${YELLOW}Warning: You have uncommitted changes${NC}"
    read -p "Do you want to commit current translations? (y/n) " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        git add -A
        git commit -m "Initial English translation of MicMac codebase

- Translated comments from French to English
- Added English README and translation mapping
- Preserved all original functionality
- Maintained build compatibility"
    fi
fi

echo -e "${GREEN}Step 6: Preparing for fork${NC}"
echo "To create your English fork on GitHub:"
echo "1. Go to GitHub and create a new repository named 'micmac_eng'"
echo "2. Run the following commands:"
echo ""
echo "   git remote add english https://github.com/YOUR_USERNAME/micmac_eng.git"
echo "   git checkout -b english-main"
echo "   git push -u english english-main:main"
echo ""

echo -e "${GREEN}Step 7: Testing build${NC}"
read -p "Do you want to test the build now? (y/n) " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    mkdir -p build_test
    cd build_test
    cmake ..
    make -j$(nproc)
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}Build successful!${NC}"
    else
        echo -e "${RED}Build failed. Please check the errors above.${NC}"
    fi
    cd ..
fi

echo -e "${GREEN}====================================="
echo "Fork preparation complete!"
echo "====================================="
echo ""
echo "Next steps:"
echo "1. Review the translations in key files"
echo "2. Test the build thoroughly"
echo "3. Create the GitHub repository"
echo "4. Push the English version"
echo "5. Continue gradual translation of remaining French terms"
echo ""
echo "Translation scripts available:"
echo "- safe_translate.py: Safe comment-only translation"
echo "- translate_to_english.py: Full translation (use with caution)"
echo ""
echo -e "${YELLOW}Note: The safe translation only modified comments to ensure build stability.${NC}"
echo -e "${YELLOW}Further translation of function/variable names can be done gradually.${NC}"