FROM gcc:11.2

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install -y cmake libgtest-dev libboost-test-dev time && rm -rf /var/lib/apt/lists/*
COPY entry_point.sh /
ENTRYPOINT ["bash", "/entry_point.sh"]