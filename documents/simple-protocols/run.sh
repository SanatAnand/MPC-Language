#!/usr/bin/env bash
# Run this script to compile OTT definitions and latex to generate a PDF file
ott -tex_show_meta false -tex_show_categories true -tex_wrap false -i ../ott-definitions/grammar.ott -o grammar.tex && latexmk -pdf language-design.tex
