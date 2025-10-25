FROM ubuntu:22.04

RUN apt-get update && \
apt-get install -y build-essential cmake gdb && \
apt-get clean

WORKDIR /app
COPY . /app
RUN make clean && make

# -s: syntax analyzer, -p: parser, -t:typechecker
CMD ["./build/autolangparser", "examples/example.alang", "-p"]