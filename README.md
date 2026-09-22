# Morph Browser

Lightweight web browser tailored for Ubuntu Touch, based on the Qt WebEngine
and using the Lomiri UI components.

## i18n: Translating Morph Browser into your Language

You can easily contribute to the localization of this project (i.e. the
translation into your language) by visiting (and signing up with) the
[Hosted Weblate service](https://hosted.weblate.org/projects/lomiri/morph-browser)

The localization platform of this project is sponsored by Hosted Weblate
via their free hosting plan for Libre and Open Source Projects.

## Copyright

Morph Browser is distributed under the GPLv3 license, see COPYING.

Morph Icon is based on Origami Licensed CC-BY by Creative Mania from the Noun
Project, see https://creativecommons.org/licenses/by/3.0/ for the license
terms.

## Building

The build system uses CMake. Please see [debian/control](debian/control) for the
complete list of build requirements. To build and run a morph debug build with
Qt6, run the following commands:

```sh
cmake -S . -B build -DCLICK_MODE=ON -DENABLE_QT6=ON -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j $(nproc)
DESTDIR=install cmake --install build
cd install
./morph-browser
```

### Building with clickable

As an alternative to setting up a build environment and running CMake commands
manually, the [clickable](https://clickable-ut.dev/en/latest/) command line tool
can be used to build morph with Qt5. For building morph with Qt6 and clickable,
see [morph-browser-qt6-click](https://gitlab.com/ubports/development/apps/morph-browser-qt6-click).

## Unit tests

To run the unit tests, you can use the commands below:

```sh
make test
# or
ctest
```

## Tests

### Code coverage

To generate a report with detailed code coverage, you need to re-run cmake with
`CMAKE_BUILD_TYPE=coverage`:

```sh
cmake -DCMAKE_BUILD_TYPE=coverage .
make
make test
make coverage
```

This will generate a coverage report in XML format (coverage.xml) and an
interactive human-readable report in HTML format (coveragereport/index.html).

## Settings

webbrowser-app supports a limited set of custom settings, persisted on disk in
the following INI-like file: `$HOME/.config/webbrowser-app/webbrowser-app.conf`

The following keys are supported:

- `homepage`: a URL that the browser will open when launched if no URL is
  specified on the command line

- `searchengine`: a custom search engine specification, looked up in
  $HOME/.local/share/webbrowser-app/searchengines/{value}.xml and following
  the [OpenSearch document description format](http://www.opensearch.org/Specifications/OpenSearch/1.1)

- `restoreSession`: whether to restore the previous browsing session at startup
  (defaults to true)
