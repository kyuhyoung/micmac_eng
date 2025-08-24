#!/usr/bin/env python3
"""
Continue MicMac Translation Script
This script provides more advanced translation capabilities for the MicMac codebase
"""

import os
import re
import sys
from pathlib import Path
import argparse
import shutil
from typing import Dict, List, Set

# Advanced translation patterns for function and class names
ADVANCED_TRANSLATIONS = {
    # Class names (with 'c' prefix)
    'cAppliApero': 'cAdjustmentApplication',
    'cAppliSaisie': 'cCaptureApplication', 
    'cPoseCam': 'cCameraPose',
    'cCalibCam': 'cCameraCalibration',
    'cStatObs': 'cObservationStatistics',
    'cElNuage3D': 'c3DPointCloud',
    'cElPolygone': 'cPolygon',
    'cElTriangle': 'cTriangle',
    'cCpleImMasq': 'cImageMaskPair',
    
    # Function names - common patterns
    'LireParametres': 'ReadParameters',
    'EcrireResultats': 'WriteResults',
    'CalculerResidus': 'ComputeResiduals',
    'OptimiserPose': 'OptimizePose',
    'TriangulerPoints': 'TriangulatePoints',
    'BasculerCoordonnees': 'TransformCoordinates',
    'SauverOrientation': 'SaveOrientation',
    'ChargerCalibration': 'LoadCalibration',
    'VerifierCoherence': 'CheckConsistency',
    'InitialiserCamera': 'InitializeCamera',
    
    # Member variable patterns
    'mNbAppuis': 'mNumControlPoints',
    'mNbImages': 'mNumImages', 
    'mNbPoints': 'mNumPoints',
    'mListeImages': 'mImageList',
    'mVecResidus': 'mResidualVector',
    'mMatriceRotation': 'mRotationMatrix',
    'mCentreOptique': 'mOpticalCenter',
    'mDistanceFocale': 'mFocalLength',
    
    # XML and configuration terms
    'DicoCamera': 'CameraDictionary',
    'ParamApero': 'AdjustmentParameters',
    'OrientationImage': 'ImageOrientation',
    'PointsAppuis': 'ControlPoints',
    'MesuresImage': 'ImageMeasurements',
    'ModeleNumerique': 'DigitalModel',
    
    # File and directory patterns
    'RepertoireImages': 'ImageDirectory',
    'FichierOrientation': 'OrientationFile',
    'NomImage': 'ImageName',
    'ExtensionFichier': 'FileExtension',
    'CheminFichier': 'FilePath',
}

# Patterns that should NOT be translated (system/library terms)
PROTECTED_PATTERNS = {
    # C++ standard library
    'std', 'vector', 'string', 'iostream', 'fstream', 'sstream',
    'map', 'set', 'list', 'queue', 'stack', 'pair', 'shared_ptr',
    
    # OpenCV
    'cv', 'Mat', 'Point', 'Rect', 'Size', 'Scalar',
    
    # Qt
    'Qt', 'QWidget', 'QApplication', 'QString', 'QMainWindow',
    
    # Mathematical terms (already in English)
    'sin', 'cos', 'tan', 'sqrt', 'pow', 'exp', 'log',
    'min', 'max', 'abs', 'floor', 'ceil',
    
    # File formats
    'jpg', 'jpeg', 'png', 'tiff', 'tif', 'xml', 'txt', 'csv',
    
    # Common abbreviations that should stay
    'GPS', 'UTM', 'WGS84', 'EPSG', 'GCP', 'DEM', 'DSM', 'RGB'
}

def is_protected_term(term: str) -> bool:
    """Check if a term should not be translated"""
    return term.lower() in [p.lower() for p in PROTECTED_PATTERNS]

def translate_advanced_identifier(identifier: str) -> str:
    """Advanced translation for identifiers"""
    if is_protected_term(identifier):
        return identifier
    
    # Check for exact matches first
    if identifier in ADVANCED_TRANSLATIONS:
        return ADVANCED_TRANSLATIONS[identifier]
    
    # Check for partial matches (compound words)
    result = identifier
    for french, english in sorted(ADVANCED_TRANSLATIONS.items(), key=lambda x: len(x[0]), reverse=True):
        # Use word boundaries for replacement
        pattern = r'\b' + re.escape(french) + r'\b'
        result = re.sub(pattern, english, result)
    
    return result

def analyze_file_for_translation_candidates(file_path: str) -> List[str]:
    """Analyze a file to find translation candidates"""
    candidates = []
    
    try:
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        # Find class definitions
        class_matches = re.findall(r'class\s+(\w*[A-Z][a-z]*\w*)', content)
        candidates.extend(class_matches)
        
        # Find function definitions
        func_matches = re.findall(r'(?:void|int|bool|double|string|auto)\s+(\w*[A-Z][a-z]*\w*)\s*\(', content)
        candidates.extend(func_matches)
        
        # Find member variables
        member_matches = re.findall(r'm[A-Z]\w*', content)
        candidates.extend(member_matches)
        
    except Exception as e:
        print(f"Error analyzing {file_path}: {e}")
    
    return list(set(candidates))  # Remove duplicates

def progressive_translate_file(file_path: str, dry_run: bool = False) -> bool:
    """Progressively translate a file with safety checks"""
    try:
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        original_content = content
        
        # Apply translations carefully
        for french, english in ADVANCED_TRANSLATIONS.items():
            if french in content:
                # Only replace if it's not in comments or strings
                lines = content.split('\n')
                new_lines = []
                
                for line in lines:
                    # Skip comment lines and string literals
                    if line.strip().startswith('//') or line.strip().startswith('*') or '/*' in line:
                        new_lines.append(line)
                        continue
                    
                    # Skip lines with string literals containing the term
                    if f'"{french}"' in line or f"'{french}'" in line:
                        new_lines.append(line)
                        continue
                    
                    # Apply translation with word boundaries
                    pattern = r'\b' + re.escape(french) + r'\b'
                    translated_line = re.sub(pattern, english, line)
                    new_lines.append(translated_line)
                
                content = '\n'.join(new_lines)
        
        changes_made = content != original_content
        
        if changes_made and not dry_run:
            # Create backup
            backup_path = f"{file_path}.adv_backup"
            if not os.path.exists(backup_path):
                shutil.copy2(file_path, backup_path)
            
            # Write translated content
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write(content)
            
            print(f"  Advanced translation: {file_path}")
        elif changes_made:
            print(f"  Would apply advanced translation: {file_path}")
        
        return changes_made
        
    except Exception as e:
        print(f"  Error: {file_path}: {e}")
        return False

def generate_translation_report(directory: str):
    """Generate a report of potential translations"""
    print("Analyzing codebase for translation opportunities...")
    
    all_candidates = set()
    file_count = 0
    
    for root, dirs, files in os.walk(directory):
        # Skip build directories
        dirs[:] = [d for d in dirs if not d.startswith('.') and d not in ['build', 'build_eng', 'CMakeFiles']]
        
        for file in files:
            if file.endswith(('.cpp', '.h', '.c', '.hpp')):
                file_path = os.path.join(root, file)
                candidates = analyze_file_for_translation_candidates(file_path)
                all_candidates.update(candidates)
                file_count += 1
    
    print(f"\nAnalyzed {file_count} files")
    print(f"Found {len(all_candidates)} potential translation candidates:")
    
    # Sort candidates by likelihood of being French
    french_likely = []
    english_likely = []
    
    for candidate in sorted(all_candidates):
        # Heuristic: contains French-like patterns
        if any(pattern in candidate.lower() for pattern in ['appui', 'bascul', 'saisie', 'masq', 'orient', 'mesur', 'residu', 'triangul']):
            french_likely.append(candidate)
        else:
            english_likely.append(candidate)
    
    print(f"\nLikely French terms ({len(french_likely)}):")
    for term in french_likely[:20]:  # Show first 20
        print(f"  {term}")
    if len(french_likely) > 20:
        print(f"  ... and {len(french_likely) - 20} more")
    
    print(f"\nPossible English terms ({len(english_likely)}):")
    for term in english_likely[:10]:  # Show first 10
        print(f"  {term}")
    if len(english_likely) > 10:
        print(f"  ... and {len(english_likely) - 10} more")

def main():
    parser = argparse.ArgumentParser(description='Continue MicMac English translation')
    parser.add_argument('path', help='Path to translate')
    parser.add_argument('--dry-run', action='store_true', help='Preview changes')
    parser.add_argument('--report', action='store_true', help='Generate translation report')
    parser.add_argument('--advanced', action='store_true', help='Apply advanced translations')
    
    args = parser.parse_args()
    
    if args.report:
        generate_translation_report(args.path)
        return
    
    if args.advanced:
        count = 0
        for root, dirs, files in os.walk(args.path):
            dirs[:] = [d for d in dirs if not d.startswith('.') and d not in ['build', 'build_eng', 'CMakeFiles']]
            
            for file in files:
                if file.endswith(('.cpp', '.h', '.c', '.hpp')):
                    file_path = os.path.join(root, file)
                    if progressive_translate_file(file_path, args.dry_run):
                        count += 1
        
        print(f"\nAdvanced translation {'would apply to' if args.dry_run else 'applied to'} {count} files")
    else:
        print("Use --report to analyze or --advanced to apply translations")

if __name__ == '__main__':
    main()