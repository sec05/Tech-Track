import aiohttp
import asyncio
import time
URL = "http://localhost:8008/data/facebook"  
NUM_REQUESTS = 100000 

async def fetch(session, url, success_count, failure_count,time_count):
    start = time.time()
    try:
        async with session.get(url) as response:
            if response.status == 200:
                success_count.append(1)
            else:
                failure_count.append(1)
            await response.text()
    except:
        failure_count.append(1)
    end = time.time()
    time_count.append(end-start)
async def main():
    success_count = []
    failure_count = []
    time_count = []
    async with aiohttp.ClientSession() as session:
        tasks = [fetch(session, URL, success_count, failure_count, time_count) for _ in range(NUM_REQUESTS)]
        await asyncio.gather(*tasks)
        print(f"{__file__.split("/")[-1]}: All requests completed. Success: {len(success_count)}, Failures: {len(failure_count)}, Average Response Time: {sum(time_count)/len(time_count)}")

if __name__ == "__main__":
    asyncio.run(main())
