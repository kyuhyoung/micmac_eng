#!/usr/bin/env python3
"""
Safe MicMac French to English Translation Script
This version is more conservative and avoids breaking preprocessor directives
"""

import os
import re
import sys
from pathlib import Path
import argparse
import shutil
from typing import Dict, List, Tuple

# Core translation dictionary - only the most essential terms
CORE_TRANSLATIONS = {
    # Critical photogrammetry terms that appear in user-facing APIs
    'Appui': 'ControlPoint',
    'appui': 'controlPoint',
    'Bascule': 'Transform',
    'bascule': 'transform',
    'Homol': 'Correspondence',
    'homol': 'correspondence',
    'Saisie': 'Capture',
    'saisie': 'capture',
    'Masq': 'Mask',
    'masq': 'mask',
    'Orient': 'Orientation',
    'orient': 'orientation',
    'Mesure': 'Measurement',
    'mesure': 'measurement',
    'Residu': 'Residual',
    'residu': 'residual',
    'Compens': 'Adjustment',
    'compens': 'adjustment',
    'Triangul': 'Triangulation',
    'triangul': 'triangulation',
    'Calib': 'Calibration',
    'calib': 'calibration',
}

# Comment translations for documentation
COMMENT_FR_TO_EN = {
    # Basic words
    'et': 'and',
    'ou': 'or',
    'mais': 'but',
    'avec': 'with',
    'sans': 'without',
    'pour': 'for',
    'dans': 'in',
    'sur': 'on',
    'sous': 'under',
    'entre': 'between',
    'apres': 'after',
    'avant': 'before',
    'si': 'if',
    'alors': 'then',
    'sinon': 'else',
    'quand': 'when',
    'fichier': 'file',
    'dossier': 'folder',
    'repertoire': 'directory',
    'image': 'image',
    'photo': 'photo',
    'camera': 'camera',
    'point': 'point',
    'ligne': 'line',
    'calcul': 'computation',
    'calcule': 'compute',
    'calculer': 'compute',
    'resultat': 'result',
    'erreur': 'error',
    'avertissement': 'warning',
    'parametres': 'parameters',
    'parametre': 'parameter',
    'methode': 'method',
    'fonction': 'function',
    'classe': 'class',
    'objet': 'object',
    'valeur': 'value',
    'nombre': 'number',
    'entier': 'integer',
    'reel': 'real',
    'chaine': 'string',
    'tableau': 'array',
    'liste': 'list',
    'vecteur': 'vector',
    'matrice': 'matrix',
    'coordonnees': 'coordinates',
    'systeme': 'system',
    'reference': 'reference',
    'transformation': 'transformation',
    'rotation': 'rotation',
    'translation': 'translation',
    'echelle': 'scale',
}

def create_backup(file_path: str):
    """Create a backup of the original file"""
    backup_path = f"{file_path}.french_backup"
    if not os.path.exists(backup_path):
        shutil.copy2(file_path, backup_path)
        return backup_path
    return None

def is_preprocessor_line(line: str) -> bool:
    """Check if a line is a preprocessor directive"""
    stripped = line.strip()
    return stripped.startswith('#') and any(d in stripped for d in ['if', 'def', 'pragma', 'include', 'define', 'endif', 'else', 'elif'])

def translate_comment_words(text: str) -> str:
    """Translate French words in comments"""
    result = text
    for fr, en in COMMENT_FR_TO_EN.items():
        # Use word boundaries for whole word replacement
        pattern = r'\b' + re.escape(fr) + r'\b'
        result = re.sub(pattern, en, result, flags=re.IGNORECASE)
    return result

def translate_identifier_safe(identifier: str) -> str:
    """Safely translate an identifier"""
    # Skip if it's a C++ keyword or standard library identifier
    cpp_keywords = {
        'if', 'else', 'for', 'while', 'do', 'switch', 'case', 'default',
        'break', 'continue', 'return', 'class', 'struct', 'enum', 'union',
        'public', 'private', 'protected', 'virtual', 'override', 'final',
        'const', 'static', 'extern', 'inline', 'template', 'typename',
        'namespace', 'using', 'typedef', 'sizeof', 'new', 'delete',
        'try', 'catch', 'throw', 'this', 'true', 'false', 'nullptr',
        'int', 'float', 'double', 'char', 'bool', 'void', 'auto',
        'unsigned', 'signed', 'long', 'short', 'std', 'vector', 'string',
        'cout', 'cin', 'endl', 'printf', 'scanf', 'malloc', 'free',
        'FILE', 'NULL', 'size_t', 'assert', 'exit', 'main'
    }
    
    if identifier in cpp_keywords:
        return identifier
    
    # Apply translations
    result = identifier
    for fr, en in CORE_TRANSLATIONS.items():
        # Only replace whole words
        if identifier == fr:
            return en
        # Handle compound identifiers like "mAppui" -> "mControlPoint"
        pattern = r'\b' + re.escape(fr) + r'\b'
        result = re.sub(pattern, en, result)
    
    return result

def translate_line_safe(line: str) -> str:
    """Safely translate a line of code"""
    # Don't touch preprocessor directives
    if is_preprocessor_line(line):
        return line
    
    # Handle single-line comments
    if '//' in line:
        code_part, comment_part = line.split('//', 1)
        # Translate comment
        translated_comment = translate_comment_words(comment_part)
        # For now, don't translate code part to be safe
        return code_part + '//' + translated_comment
    
    # Handle block comments
    if '/*' in line or '*/' in line:
        return translate_comment_words(line)
    
    # For regular code, be very conservative
    # Only translate if it's clearly a variable/function name
    # This is intentionally limited to avoid breaking code
    
    return line

def translate_file_safe(file_path: str, dry_run: bool = False) -> bool:
    """Safely translate a single file"""
    try:
        # Read the file
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        # Only translate comments for safety
        lines = content.split('\n')
        translated_lines = []
        changes_made = False
        
        in_block_comment = False
        for line in lines:
            if '/*' in line:
                in_block_comment = True
            
            if in_block_comment or '//' in line:
                translated_line = translate_line_safe(line)
                if translated_line != line:
                    changes_made = True
                translated_lines.append(translated_line)
            else:
                # Keep code as-is for now
                translated_lines.append(line)
            
            if '*/' in line:
                in_block_comment = False
        
        if changes_made and not dry_run:
            # Create backup
            backup_path = create_backup(file_path)
            if backup_path:
                print(f"  Backup: {backup_path}")
            
            # Write translated content
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write('\n'.join(translated_lines))
            
            print(f"  Translated: {file_path}")
        elif changes_made:
            print(f"  Would translate: {file_path}")
        
        return changes_made
    
    except Exception as e:
        print(f"  Error: {file_path}: {e}")
        return False

def main():
    parser = argparse.ArgumentParser(description='Safe translation of MicMac from French to English')
    parser.add_argument('path', help='Path to translate')
    parser.add_argument('--dry-run', action='store_true', help='Preview changes')
    parser.add_argument('--restore', action='store_true', help='Restore from backup')
    
    args = parser.parse_args()
    
    if args.restore:
        # Restore logic
        count = 0
        for root, dirs, files in os.walk(args.path):
            for file in files:
                if file.endswith('.french_backup'):
                    original = file[:-14]
                    full_original = os.path.join(root, original)
                    full_backup = os.path.join(root, file)
                    shutil.copy2(full_backup, full_original)
                    count += 1
        print(f"Restored {count} files")
    else:
        # Translation logic
        count = 0
        for root, dirs, files in os.walk(args.path):
            # Skip build directories
            dirs[:] = [d for d in dirs if not d.startswith('.') and d not in ['build', 'build_eng', 'CMakeFiles']]
            
            for file in files:
                if file.endswith(('.cpp', '.h', '.c', '.hpp')):
                    file_path = os.path.join(root, file)
                    if translate_file_safe(file_path, args.dry_run):
                        count += 1
        
        print(f"\nTranslated {count} files (comments only for safety)")
        if not args.dry_run:
            print("Note: This safe version only translates comments to avoid breaking builds")
            print("Use --restore to revert changes")

if __name__ == '__main__':
    main()