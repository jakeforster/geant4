;;
;; Setup an environment in which to build and run Geant4 applications.
;; Use this manifest when no visualisation is required.
;; For visualisation, use manifest.scm instead.

;; Usage, e.g.: guix shell -C -m manifest-novis.scm
;; Requirements:
;; - install the GNU Guix package manager
;; - add the Guix channel at: https://github.com/jakeforster/guix-channel

;; for clangd to find your source files:
;; cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
;; or
;; cmake --preset default ..
;; then
;; ln -s build/compile_commands.json compile_commands.json

(specifications->manifest '("geant4"
			    "cmake"
			    "make"
			    "gcc-toolchain"

			    "clang" ;; M-x eglot, provides clangd

			    "coreutils" ;; for when using container -C 
                         ))

