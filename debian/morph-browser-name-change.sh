#!/bin/sh

morph_dir="${XDG_DATA_HOME:-${HOME}/.local/share}/morph-browser"
webbrowser_dir="${XDG_DATA_HOME:-${HOME}/.local/share}/webbrowser-app"

if [ -d "${webbrowser_dir}" ] && [ ! -d "${morph_dir}" ]; then
    mkdir -p "${morph_dir} || exit 1
    cp "${webbrowser_dir}/bookmarks.sqlite" \
        "${webbrowser_dir}/downloads.sqlite" \
        "${webbrowser_dir}/history.sqlite" \
        "${morph_dir}"
fi
