# Tau SDSKV Plugin
The TAU SDSKV Plugin logs TAU Profiling events to an SDSKV Key-Value Database.

## Dependencies
The TAU SDSKV Plugin depends on these libraries, which can all be installed via
[Spack](https://spack.io):

* Repo: builtin
    * [argobots](http://www.argobots.org/) 
    * [mercury](https://mercury-hpc.github.io/)
    * [tau](http://www.cs.uoregon.edu/research/tau)
* Repo: [sds-repo](https://xgitlab.cels.anl.gov/sds/sds-repo)
    * [margo](https://xgitlab.cels.anl.gov/sds/margo)
    * [sdskeyval](https://xgitlab.cels.anl.gov/sds/sds-keyval)

It is recommended to install these packages using Spack, but you should also be
able to install them via their typical install mechanisms.

## Building
CMake is used to build the plugin. Configuration and build is typical:
```bash
mkdir build
cd build
cmake ..
make -j
```

TODO: Support install

## How To Use
The TAU SDSKV Plugin is registered like any other TAU Plugin - you must set the
`TAU_PLUGINS` and `TAU_PLUGINS_PATH` environment variables. e.g.:

```
export TAU_PLUGINS_PATH=/home/agaspar/Code/tau-sdskeyval/build/plugin/
export TAU_PLUGINS=libtau_sdskeyval_plugin.so
```

In addition, you need to tell the plugin which SDSKV daemon to connect to using
`TAU_SDSKV_SERVER` and `TAU_SDSKV_DB`.

For example, imagine you launched the `sdskv-server-daemon` like so:
```bash
sdskv-server-daemon tcp://127.0.0.1:1234 tau:bdb &
```

You can tell the TAU SDSKV Plugin to connect to the "tau" database at
"tcp://127.0.0.1:1234" with these settings:

```
export TAU_SDSKV_SERVER=tcp://127.0.0.1:1234
export TAU_SDSKV_DB=tau
```

### Prefix Name (Optional)
If you'd like to set a per-dump prefix name, you'll need to link
`tau_sdskeyval_control`. When you've done that, you can call
`tausdskeyval_set_dump_name` in `tau-dskeyval/plugin.h`.

```c++
tausdskeyval_set_dump_name("my_dump_name");
```

An example use case is to call this once per simulation time step prior to a
call to `Tau_dump` with a name unique to the time-step.