;;
;; Setup an environment in which to build and run Geant4 applications.
;; Use this manifest when visualisation is required.

;; Usage, e.g.: guix shell -m manifest.scm
;; Requirements:
;; - install the GNU Guix package manager
;; - add the Guix channel at: https://github.com/jakeforster/guix-channel

;; for clangd to find your source files:
;; cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
;; or
;; cmake --preset default ..
;; then
;; ln -s build/compile_commands.json compile_commands.json

(specifications->manifest '("geant4-vis"  
			    "cmake"
			    "make"
			    "gcc-toolchain"
			    "mesa"

			    "clang" ;; M-x eglot, provides clangd

			    "dawn"
			    ))

