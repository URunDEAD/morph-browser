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

**NOTE**: autopilot is currently not available in UBports and the instructions
below are no longer accurate.

### Automated UI tests

webbrowser-app uses [autopilot](https://launchpad.net/autopilot) to test its UI.
To run the tests locally, you will need to install python3-autopilot and
autopilot-qt5.
Then do the following:

```sh
cd tests/autopilot/
autopilot3 run webbrowser_app
```

You can get a list of all available tests with the following command:

```sh
autopilot3 list webbrowser_app
```

In order to run the tests in a virtual machine with an environment closer to
what a user will get in Ubuntu Touch, see the Dep8 tests section.

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

### Dep8 tests

Dep8 tests exercise the package "as-installed".

Currently, the webbrowser-app has one suite of dep8 tests that uses
[autopilot](https://launchpad.net/autopilot) to test from the point of view of
the user.

To run the tests you will need autopkgtest:

```sh
sudo apt-get install autopkgtest
```

You can use multiple test beds to execute the tests. Below you will find
instructions to run them in a virtual machine
You can find more information with:

```sh
man adt-run
```

#### Run dep8 tests

To run the tests in a qemu virtual machine, you will first have to create it
(see /usr/share/doc/autopkgtest/README.running-tests.rst.gz). We output the
image to ~/ rather than the current directory, so it will be in a safer
place to avoid rebuilding images every time. You can store it in any
directory you wish. This image is better consumed "fresh", building it daily
will avoid long updates/upgrades when running the tests.

```sh
adt-buildvm-ubuntu-cloud -r $(lsb_release -c -s) -a amd64 -o ~/
```

Then run the tests using adt-run with the qemu virtualization host against
the current archive.

```sh
adt-run -B -U --unbuilt-tree . \
  -o ~/adt-browser-test/$(date +%Y-%m-%d-%H-%M) \
  --- qemu ~/adt-$(lsb_release -c -s)-amd64-cloud.img
```

The tests can also be run on a local phone.

```sh
adt-run -B -U --unbuilt-tree . \
  -o ~/adt-browser-test/$(date +%Y-%m-%d-%H-%M) \
  --- ssh -s adb -- -p <password> --serial <ADB_SERIAL>
```

#### Examine the dep8 autopilot results

To examine the test results, which are in subunit format, additional tools are
required, such as [trv](https://launchpad.net/trv).

You can find the results file in the directory
`~/adt-browser-test/$(date +%Y-%m-%d-%H-%M)/artifacts`.

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
