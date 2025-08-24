# French to English Translation Mapping

This document maps the French terms used in the original MicMac to their English equivalents in this fork.

## Core Photogrammetry Terms

| French | English | Context | Example Usage |
|--------|---------|---------|---------------|
| Appui | ControlPoint | Ground control points | `cAppliSaisieAppuis` → `cControlPointCaptureApp` |
| Bascule | Transform | Coordinate transformation | `Bascule3D` → `Transform3D` |
| Calib | Calibration | Camera calibration | `CalibCamera` → `CameraCalibration` |
| Compens | Adjustment | Bundle adjustment | `Compensation` → `BundleAdjustment` |
| Coord | Coordinate | Coordinates | `CoordSys` → `CoordinateSystem` |
| Homol | Correspondence | Feature correspondences | `Homologue` → `Correspondence` |
| Masq/Masque | Mask | Image masks | `MasqueImage` → `ImageMask` |
| Mes/Mesure | Measurement | Observations | `MesureAppui` → `ControlPointMeasurement` |
| Orient | Orientation | Camera orientation | `OrientationCamera` → `CameraOrientation` |
| Repere | Reference | Reference frame | `RepereLocal` → `LocalReference` |
| Residu | Residual | Error residuals | `ResiduMoyen` → `MeanResidual` |
| Saisie | Capture | Data capture/input | `SaisiePoints` → `PointCapture` |
| Som/Sommet | Vertex | Vertex/summit | `SommetTriangle` → `TriangleVertex` |
| Triangul | Triangulation | Triangulation | `TriangulationAuto` → `AutoTriangulation` |

## Common Variable Prefixes

| French | English | Usage Example | Translation |
|--------|---------|---------------|-------------|
| aNb | aNum | `aNbPoints` | `aNumPoints` |
| mNb | mNum | `mNbImages` | `mNumImages` |
| aPt | aPoint | `aPtCible` | `aTargetPoint` |
| mPt | mPoint | `mPtAppui` | `mControlPoint` |
| aVec | aVec | `aVecNormal` | `aNormalVec` |
| aRes | aResult | `aResCalcul` | `aCompResult` |
| aSom | aSum | `aSomErreur` | `aErrorSum` |

## File and Process Operations

| French | English | Context |
|--------|---------|---------|
| Fichier | File | File operations |
| Lire | Read | Reading data |
| Ecrire | Write | Writing data |
| Ouvrir | Open | Opening files |
| Fermer | Close | Closing resources |
| Charger | Load | Loading data |
| Sauver | Save | Saving data |
| Calcul | Computation | Mathematical operations |
| Choix | Choice | Selection/choice |
| Champ | Field | Data fields |

## Geometric Terms

| French | English | Usage |
|--------|---------|-------|
| Cercle | Circle | Geometric circles |
| Droite | Line | Straight lines |
| Polygone | Polygon | Polygon shapes |
| Carre | Square | Square shapes |
| Rectangle | Rectangle | Rectangular shapes |
| Noeud | Node | Graph nodes |
| Grille | Grid | Data grids |
| Maille | Mesh | 3D meshes |

## Image Processing Terms

| French | English | Context |
|--------|---------|---------|
| Couleur | Color | Color processing |
| Niveau | Level | Intensity levels |
| Gris | Gray | Grayscale |
| Echelle | Scale | Image scaling |
| Moyenne | Average | Statistical averages |
| Ecart | Deviation | Statistical deviation |
| Poids | Weight | Weighting factors |

## Mathematical and Statistical Terms

| French | English | Usage |
|--------|---------|-------|
| Moyenne | Average | `MoyenneQuadratique` → `RootMeanSquare` |
| Ecart | Deviation | `EcartType` → `StandardDeviation` |
| Poids | Weight | `PoidsObservation` → `ObservationWeight` |
| Valeur | Value | `ValeurPixel` → `PixelValue` |
| Nombre | Number | `NombreIterations` → `NumberIterations` |

## Directional Terms

| French | English |
|--------|---------|
| Gauche | Left |
| Droite | Right |
| Haut | Top |
| Bas | Bottom |
| Nord | North |
| Sud | South |
| Est | East |
| Ouest | West |

## Boolean and State Terms

| French | English |
|--------|---------|
| Vrai | True |
| Faux | False |
| Actif | Active |
| Inactif | Inactive |
| Valide | Valid |
| Invalide | Invalid |

## Common Verbs in Function Names

| French | English | Example Translation |
|--------|---------|-------------------|
| Ajouter | Add | `AjouterPoint` → `AddPoint` |
| Supprimer | Remove | `SupprimerElement` → `RemoveElement` |
| Chercher | Search | `ChercherCorrespondance` → `SearchCorrespondence` |
| Trouver | Find | `TrouverSolution` → `FindSolution` |
| Creer | Create | `CreerMaillage` → `CreateMesh` |
| Modifier | Modify | `ModifierParametre` → `ModifyParameter` |

## XML Configuration Elements

| French XML Element | English Translation | Purpose |
|-------------------|-------------------|---------|
| `BasculeOrientation` | `OrientationTransform` | Orientation transformation |
| `BDD_ObsAppuisFlottant` | `ControlPointObservations` | Control point database |
| `CalibrationCameraInc` | `CameraCalibrationParameters` | Camera calibration |
| `EtapeCompensation` | `AdjustmentStep` | Bundle adjustment step |
| `ObsAppuisFlottant` | `ControlPointObservations` | Control point observations |
| `PoseCameraInc` | `CameraPoseParameters` | Camera pose parameters |
| `SectionInconnues` | `UnknownParametersSection` | Unknown parameters |
| `SectionObservations` | `ObservationsSection` | Observations section |

## Translation Principles

1. **Maintain Technical Accuracy**: All translations preserve the technical meaning
2. **Follow Photogrammetry Standards**: Use standard English photogrammetry terminology
3. **Consistency**: Same French term always translates to same English term
4. **Readability**: Prefer readable English over literal translations
5. **Compatibility**: Ensure translations don't break existing workflows

## Implementation Status

- ✅ **Comments**: Translated in 986 files
- 🚧 **Variable Names**: Partial translation in progress
- 🚧 **Function Names**: Planned for next phase
- 🚧 **XML Elements**: Planned for future release
- 🚧 **Documentation**: In progress

## Usage in Translation Scripts

This mapping is used by:
- `safe_translate.py`: Comment translation only
- `translate_to_english.py`: Full code translation (use with caution)
- `prepare_english_fork.sh`: Complete workflow

## Contributing to Translations

When adding new translations:
1. Follow the established patterns
2. Check for consistency with existing translations
3. Test build compatibility
4. Update this mapping document
5. Use standard photogrammetry terminology

This mapping ensures consistent translation across the entire MicMac codebase while maintaining full functionality and compatibility.