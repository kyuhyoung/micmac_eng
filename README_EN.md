# MicMac English Fork (micmac_eng)

This is an English translation fork of the MicMac photogrammetry software originally developed by IGN (Institut Géographique National).

## About This Fork

This fork aims to make MicMac more accessible to the English-speaking community by:
- Translating French variable names, function names, and comments to English
- Maintaining full compatibility with the original MicMac functionality  
- Preserving all original algorithms and processing capabilities

## Original Project

The original MicMac project is maintained by IGN France:
- Official website: https://micmac.ign.fr
- Original repository: https://github.com/micmacIGN/micmac

## Translation Status

✅ **Completed:**
- Core source files comments translated (986 files)
- Build system maintained and functional
- Translation scripts created for further improvements
- Documentation structure prepared

🚧 **In Progress:**
- Full variable/function name translation
- XML configuration files translation
- Complete documentation translation

## Building

The build process remains the same as the original MicMac:

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### Dependencies

- CMake (>= 3.0)
- C++ compiler with C++11 support
- Qt5 (optional, for GUI tools)
- GDAL (optional, for geographic data support)

## Key Translations

| French Term | English Translation | Context |
|------------|-------------------|---------|
| Appui | ControlPoint | Ground control points |
| Bascule | Transform | Coordinate transformation |
| Homol | Correspondence | Feature matching |
| Saisie | Capture | Data input |
| Masq | Mask | Image masks |
| Orient | Orientation | Camera orientation |
| Mesure | Measurement | Observations |
| Residu | Residual | Error terms |

See [TRANSLATION_MAPPING.md](TRANSLATION_MAPPING.md) for complete translation reference.

## Usage

All MicMac commands work the same way:

```bash
mm3d [Command] [Arguments]
```

Example workflow:
```bash
# Feature extraction
mm3d Tapioca All "*.jpg" 1500

# Camera calibration
mm3d Tapas RadialStd "*.jpg" Out=Calib

# Dense matching
mm3d Malt Ortho "*.jpg" Calib
```

## Contributing

Contributions to improve translations are welcome! Please:
1. Use the provided translation scripts
2. Maintain build compatibility
3. Test changes thoroughly
4. Submit pull requests with clear descriptions

## License

This fork maintains the same CeCILL-B license as the original MicMac project.

## Acknowledgments

All credit for the original algorithms and implementation goes to:
- **Marc Pierrot Deseilligny** (IGN) - Main author
- **Gregoire Maillet** - Contributor
- **Didier Boldo** - Contributor
- And all other MicMac contributors at IGN

## Translation Tools

This repository includes translation utilities:
- `safe_translate.py` - Conservative comment translation
- `translate_to_english.py` - Comprehensive translation (use carefully)
- `prepare_english_fork.sh` - Fork preparation workflow

## Support

For issues specific to this English fork, please open an issue on GitHub.
For MicMac algorithm questions, refer to the original documentation.

---

*This English fork was created by @kyuhyoung to improve accessibility for the international photogrammetry community while maintaining full respect for the original French implementation by IGN.*