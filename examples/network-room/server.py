#!/usr/bin/env python3

import asyncio
from asyncio.streams import StreamReader, StreamWriter
from typing import List
import sys

PORT_RANGE = (15000, 15100)

class EchoServer:
    def __init__(self, port: int) -> None:
        self.port = port
        self.lock = asyncio.Lock()
        self.writers: List[asyncio.StreamWriter] = []

    async def serve(self) -> None:
        await asyncio.start_server(self.new_connection, '0.0.0.0', self.port)

    async def broadcast(self, data, exceptWriter) -> None:
        async with self.lock:
            for w in self.writers:
                if w is not exceptWriter:
                    try:
                        w.write(data)
                    except Exception as e:
                        print(e)

    async def new_connection(self, reader: StreamReader, writer: StreamWriter) -> None:
        async with self.lock:
            self.writers.append(writer)

        try:
            while True:
                data = await reader.read(1024)
                if len(data) > 0:
                    await self.broadcast(data, writer)
        except ConnectionError:
            pass
        finally:
            self.writers.remove(writer)


if __name__ == "__main__":
    try:
        loop = asyncio.new_event_loop()
        for p in range(PORT_RANGE[0], PORT_RANGE[1]):
            loop.create_task(EchoServer(p).serve())

        print(f"Serving on range {PORT_RANGE}")
        loop.run_forever()
    except KeyboardInterrupt:
        sys.exit(1)
