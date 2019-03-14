# Tests

## Usage

All tests are able to run without an external framework. They are build and executed via the `Makefile` so it is sufficient to just run

```bash
make
```

In this dir.

If there was a failure, an error code will be returned, and the affected test case will output its state.
If everything is successful, "All tests were executed successfully!" will be echoed after the tests.
