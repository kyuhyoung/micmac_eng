#!/usr/bin/env python3
"""
MicMac French to English Translation Script
Translates French variable names, function names, and comments to English
"""

import os
import re
import sys
from pathlib import Path
import argparse
import shutil
from typing import Dict, List, Tuple

# Dictionary of French to English translations for common terms
FRENCH_TO_ENGLISH = {
    # Core photogrammetry terms
    'Appui': 'ControlPoint',
    'appui': 'controlPoint',
    'Bascule': 'Transform',
    'bascule': 'transform',
    'Calib': 'Calibration',
    'calib': 'calibration',
    'Compens': 'Adjustment',
    'compens': 'adjustment',
    'Coord': 'Coordinate',
    'coord': 'coordinate',
    'Etat': 'State',
    'etat': 'state',
    'Grille': 'Grid',
    'grille': 'grid',
    'Homol': 'Correspondence',
    'homol': 'correspondence',
    'Inconnue': 'Unknown',
    'inconnue': 'unknown',
    'Liaison': 'Connection',
    'liaison': 'connection',
    'Maille': 'Mesh',
    'maille': 'mesh',
    'Masq': 'Mask',
    'masq': 'mask',
    'Mesure': 'Measurement',
    'mesure': 'measurement',
    'Mes': 'Meas',
    'mes': 'meas',
    'Noeud': 'Node',
    'noeud': 'node',
    'Obs': 'Observation',
    'obs': 'observation',
    'Orient': 'Orientation',
    'orient': 'orientation',
    'Repere': 'Reference',
    'repere': 'reference',
    'Residu': 'Residual',
    'residu': 'residual',
    'Saisie': 'Capture',
    'saisie': 'capture',
    'Sommet': 'Vertex',
    'sommet': 'vertex',
    'Som': 'Vertex',
    'som': 'vertex',
    'Triangul': 'Triangulation',
    'triangul': 'triangulation',
    
    # Common prefixes and terms
    'aNb': 'aNum',
    'mNb': 'mNum',
    'Nb': 'Num',
    'nb': 'num',
    'aVec': 'aVec',
    'aPt': 'aPoint',
    'mPt': 'mPoint',
    'aRes': 'aResult',
    'mRes': 'mResult',
    'aSom': 'aSum',
    'mSom': 'mSum',
    
    # File and process terms
    'Fichier': 'File',
    'fichier': 'file',
    'Fich': 'File',
    'fich': 'file',
    'Calcul': 'Compute',
    'calcul': 'compute',
    'Calc': 'Comp',
    'calc': 'comp',
    'Choix': 'Choice',
    'choix': 'choice',
    'Champ': 'Field',
    'champ': 'field',
    
    # Geometric terms
    'Cercle': 'Circle',
    'cercle': 'circle',
    'Droite': 'Line',
    'droite': 'line',
    'Polygone': 'Polygon',
    'polygone': 'polygon',
    'Carre': 'Square',
    'carre': 'square',
    'Rectangle': 'Rectangle',
    'rectangle': 'rectangle',
    
    # Image processing
    'Couleur': 'Color',
    'couleur': 'color',
    'Coul': 'Col',
    'coul': 'col',
    'Niveau': 'Level',
    'niveau': 'level',
    'Gris': 'Gray',
    'gris': 'gray',
    'Echelle': 'Scale',
    'echelle': 'scale',
    
    # Mathematical terms
    'Moyenne': 'Average',
    'moyenne': 'average',
    'Moy': 'Avg',
    'moy': 'avg',
    'Ecart': 'Deviation',
    'ecart': 'deviation',
    'Poids': 'Weight',
    'poids': 'weight',
    
    # Directional terms
    'Gauche': 'Left',
    'gauche': 'left',
    'Droite': 'Right',
    'droite': 'right',
    'Haut': 'Top',
    'haut': 'top',
    'Bas': 'Bottom',
    'bas': 'bottom',
    
    # Boolean and state
    'Vrai': 'True',
    'vrai': 'true',
    'Faux': 'False',
    'faux': 'false',
    
    # Common verbs in function names
    'Ajouter': 'Add',
    'ajouter': 'add',
    'Ajout': 'Add',
    'ajout': 'add',
    'Supprimer': 'Remove',
    'supprimer': 'remove',
    'Supp': 'Rem',
    'supp': 'rem',
    'Chercher': 'Search',
    'chercher': 'search',
    'Cherche': 'Search',
    'cherche': 'search',
    'Trouver': 'Find',
    'trouver': 'find',
    'Trouve': 'Find',
    'trouve': 'find',
    'Lire': 'Read',
    'lire': 'read',
    'Ecrire': 'Write',
    'ecrire': 'write',
    'Ouvrir': 'Open',
    'ouvrir': 'open',
    'Fermer': 'Close',
    'fermer': 'close',
    'Charger': 'Load',
    'charger': 'load',
    'Charge': 'Load',
    'charge': 'load',
    'Sauver': 'Save',
    'sauver': 'save',
    'Sauve': 'Save',
    'sauve': 'save',
    'Creer': 'Create',
    'creer': 'create',
    'Cree': 'Create',
    'cree': 'create',
}

# Common French comment words to translate
COMMENT_TRANSLATIONS = {
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
    'pendant': 'during',
    'si': 'if',
    'alors': 'then',
    'sinon': 'else',
    'quand': 'when',
    'nouveau': 'new',
    'ancien': 'old',
    'premier': 'first',
    'dernier': 'last',
    'suivant': 'next',
    'precedent': 'previous',
    'tous': 'all',
    'aucun': 'none',
    'quelque': 'some',
    'chaque': 'each',
    'autre': 'other',
    'meme': 'same',
    'plus': 'more',
    'moins': 'less',
    'tres': 'very',
    'trop': 'too',
    'assez': 'enough',
    'peut': 'can',
    'doit': 'must',
    'veut': 'wants',
    'fait': 'does',
    'etre': 'be',
    'avoir': 'have',
    'faire': 'do',
    'dire': 'say',
    'aller': 'go',
    'voir': 'see',
    'savoir': 'know',
    'pouvoir': 'can',
    'vouloir': 'want',
    'devoir': 'must',
    'prendre': 'take',
    'donner': 'give',
    'utiliser': 'use',
    'trouver': 'find',
    'penser': 'think',
    'comprendre': 'understand',
    'parler': 'speak',
    'mettre': 'put',
    'porter': 'carry',
    'ecrire': 'write',
    'aimer': 'like',
    # Add more as needed
}

def create_backup(file_path: str):
    """Create a backup of the original file"""
    backup_path = f"{file_path}.french_backup"
    if not os.path.exists(backup_path):
        shutil.copy2(file_path, backup_path)
        return backup_path
    return None

def translate_identifier(identifier: str, translations: Dict[str, str]) -> str:
    """Translate a single identifier (variable, function, class name)"""
    original = identifier
    
    # Sort translations by length (longest first) to avoid partial replacements
    sorted_translations = sorted(translations.items(), key=lambda x: len(x[0]), reverse=True)
    
    for french, english in sorted_translations:
        # Use word boundaries for whole word replacement
        pattern = r'\b' + re.escape(french) + r'\b'
        identifier = re.sub(pattern, english, identifier)
    
    return identifier

def translate_comment(comment: str) -> str:
    """Translate French words in comments"""
    words = comment.split()
    translated_words = []
    
    for word in words:
        # Remove punctuation for matching
        clean_word = re.sub(r'[^\w\s]', '', word.lower())
        punctuation = re.findall(r'[^\w\s]', word)
        
        if clean_word in COMMENT_TRANSLATIONS:
            # Preserve original capitalization
            if word[0].isupper():
                translated = COMMENT_TRANSLATIONS[clean_word].capitalize()
            else:
                translated = COMMENT_TRANSLATIONS[clean_word]
            
            # Re-add punctuation
            if punctuation:
                translated += ''.join(punctuation)
            translated_words.append(translated)
        else:
            translated_words.append(word)
    
    return ' '.join(translated_words)

def translate_line(line: str) -> str:
    """Translate a single line of code"""
    # Check if it's a comment line
    comment_match = re.match(r'^(\s*//\s*)(.*)$', line)
    if comment_match:
        indent, comment = comment_match.groups()
        return indent + translate_comment(comment)
    
    # Check for inline comments
    code_comment_match = re.match(r'^(.*?)(\s*//\s*)(.*)$', line)
    if code_comment_match:
        code, comment_sep, comment = code_comment_match.groups()
        translated_code = translate_code_line(code)
        translated_comment = translate_comment(comment)
        return translated_code + comment_sep + translated_comment
    
    # Check for block comments
    if '/*' in line or '*/' in line or (line.strip().startswith('*') and not line.strip().startswith('*/')):
        # For now, translate words in block comments
        return translate_comment(line)
    
    # Otherwise, translate code
    return translate_code_line(line)

def translate_code_line(line: str) -> str:
    """Translate identifiers in a line of code"""
    # Pattern to match identifiers (variable names, function names, etc.)
    identifier_pattern = r'\b([a-zA-Z_][a-zA-Z0-9_]*)\b'
    
    def replace_identifier(match):
        identifier = match.group(1)
        # Skip C++ keywords
        cpp_keywords = {'if', 'else', 'for', 'while', 'do', 'switch', 'case', 'default',
                       'break', 'continue', 'return', 'class', 'struct', 'enum', 'union',
                       'public', 'private', 'protected', 'virtual', 'override', 'final',
                       'const', 'static', 'extern', 'inline', 'template', 'typename',
                       'namespace', 'using', 'typedef', 'sizeof', 'new', 'delete',
                       'try', 'catch', 'throw', 'this', 'true', 'false', 'nullptr',
                       'int', 'float', 'double', 'char', 'bool', 'void', 'auto',
                       'unsigned', 'signed', 'long', 'short', 'std', 'vector', 'string',
                       'cout', 'cin', 'endl', 'include', 'define', 'ifdef', 'ifndef',
                       'endif', 'pragma'}
        
        if identifier in cpp_keywords:
            return identifier
        
        return translate_identifier(identifier, FRENCH_TO_ENGLISH)
    
    return re.sub(identifier_pattern, replace_identifier, line)

def translate_file(file_path: str, dry_run: bool = False) -> bool:
    """Translate a single file from French to English"""
    try:
        # Read the file
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            lines = f.readlines()
        
        # Translate each line
        translated_lines = []
        changes_made = False
        
        for line in lines:
            translated_line = translate_line(line)
            if translated_line != line:
                changes_made = True
            translated_lines.append(translated_line)
        
        if changes_made and not dry_run:
            # Create backup
            backup_path = create_backup(file_path)
            if backup_path:
                print(f"  Backup created: {backup_path}")
            
            # Write translated content
            with open(file_path, 'w', encoding='utf-8') as f:
                f.writelines(translated_lines)
            
            print(f"  Translated: {file_path}")
        elif changes_made:
            print(f"  Would translate: {file_path}")
        
        return changes_made
    
    except Exception as e:
        print(f"  Error processing {file_path}: {e}")
        return False

def translate_directory(directory: str, extensions: List[str], dry_run: bool = False) -> int:
    """Translate all files in a directory recursively"""
    translated_count = 0
    
    for root, dirs, files in os.walk(directory):
        # Skip hidden directories and build directories
        dirs[:] = [d for d in dirs if not d.startswith('.') and d not in ['build', 'CMakeFiles']]
        
        for file in files:
            if any(file.endswith(ext) for ext in extensions):
                file_path = os.path.join(root, file)
                if translate_file(file_path, dry_run):
                    translated_count += 1
    
    return translated_count

def main():
    parser = argparse.ArgumentParser(description='Translate MicMac codebase from French to English')
    parser.add_argument('path', help='Path to file or directory to translate')
    parser.add_argument('--dry-run', action='store_true', help='Show what would be changed without making changes')
    parser.add_argument('--extensions', nargs='+', default=['.cpp', '.h', '.c', '.hpp', '.cxx', '.cc'],
                       help='File extensions to process (default: .cpp .h .c .hpp .cxx .cc)')
    parser.add_argument('--restore', action='store_true', help='Restore files from French backup')
    
    args = parser.parse_args()
    
    if args.restore:
        # Restore from backups
        if os.path.isfile(args.path):
            backup_path = f"{args.path}.french_backup"
            if os.path.exists(backup_path):
                shutil.copy2(backup_path, args.path)
                print(f"Restored {args.path} from backup")
            else:
                print(f"No backup found for {args.path}")
        else:
            # Restore all backups in directory
            count = 0
            for root, dirs, files in os.walk(args.path):
                for file in files:
                    if file.endswith('.french_backup'):
                        original_path = file[:-14]  # Remove .french_backup
                        full_original = os.path.join(root, original_path)
                        full_backup = os.path.join(root, file)
                        shutil.copy2(full_backup, full_original)
                        count += 1
            print(f"Restored {count} files from backups")
    else:
        # Translate files
        if os.path.isfile(args.path):
            translate_file(args.path, args.dry_run)
        elif os.path.isdir(args.path):
            count = translate_directory(args.path, args.extensions, args.dry_run)
            if args.dry_run:
                print(f"\nWould translate {count} files")
                print("Run without --dry-run to apply changes")
            else:
                print(f"\nTranslated {count} files")
                print("Backups created with .french_backup extension")
                print("Use --restore to revert changes")
        else:
            print(f"Error: {args.path} is neither a file nor a directory")
            sys.exit(1)

if __name__ == '__main__':
    main()