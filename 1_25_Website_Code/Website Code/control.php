<!-- component -->
<script src="https://cdn.tailwindcss.com"></script>

<aside class="ml-[-100%] fixed z-10 top-0 pb-3 px-6 w-full flex flex-col justify-between h-screen border-r bg-white transition duration-300 md:w-4/12 lg:ml-0 lg:w-[25%] xl:w-[20%] 2xl:w-[15%]">
    <div>
        <div class="-mx-6 px-6 py-1">
            <a href="#" title="home"></a>
        </div>

        <div class="text-center">
		    <img src="logo.png" alt="Italian Trulli">
			<div class="flex items-center justify-center">
			<div class="blob"></div>
			<span class="pulsate-text">System Online</span>
			</div>
<style>
			
.pulsate-text {
	color: rgba(0, 112, 0, 1);
}

.blob {
	background: rgba(0, 112, 0, 1);
	border-radius: 50%;
	margin: 10px;
	height: 20px;
	width: 20px;

	box-shadow: 0 0 0 0 rgba(0, 112, 0, 1);
	transform: scale(1);
	animation: pulse 2s infinite;
}

@keyframes pulse {
	0% {
		transform: scale(0.95);
		box-shadow: 0 0 0 0 rgba(0, 112, 0, 0.7);
	}

	70% {
		transform: scale(1);
		box-shadow: 0 0 0 10px rgba(0, 112, 0, 0);
	}

	100% {
		transform: scale(0.95);
		box-shadow: 0 0 0 0 rgba(0, 112, 0, 0);
	}
}

</style>

            <h5 class="hidden mt-4 text-xl font-semibold text-gray-600 lg:block">William Anderson</h5>
            <span class="hidden text-gray-400 lg:block">Admin</span>
        </div>

        <ul class="space-y-2 tracking-wide mt-8">
            <li>
                <a href="/dashboard.php" aria-label="dashboard" class="px-4 py-3 flex items-center space-x-4 rounded-md text-gray-600 group">
                    <svg class="-ml-1 h-6 w-6" viewBox="0 0 24 24" fill="none">
                        <path d="M6 8a2 2 0 0 1 2-2h1a2 2 0 0 1 2 2v1a2 2 0 0 1-2 2H8a2 2 0 0 1-2-2V8ZM6 15a2 2 0 0 1 2-2h1a2 2 0 0 1 2 2v1a2 2 0 0 1-2 2H8a2 2 0 0 1-2-2v-1Z" class="fill-current text-cyan-400 dark:fill-slate-600"></path>
                        <path d="M13 8a2 2 0 0 1 2-2h1a2 2 0 0 1 2 2v1a2 2 0 0 1-2 2h-1a2 2 0 0 1-2-2V8Z" class="fill-current text-cyan-200 group-hover:text-cyan-300"></path>
                        <path d="M13 15a2 2 0 0 1 2-2h1a2 2 0 0 1 2 2v1a2 2 0 0 1-2 2h-1a2 2 0 0 1-2-2v-1Z" class="fill-current group-hover:text-sky-300"></path>
                    </svg>
                    <span class="-mr-1 font-medium">Dashboard</span>
                </a>
            </li>
            <li>
                <a href="/profile.php" class="px-4 py-3 flex items-center space-x-4 rounded-md text-gray-600 group">
                    <svg xmlns="http://www.w3.org/2000/svg" class="h-5 w-5" viewBox="0 0 20 20" fill="currentColor">
                        <path class="fill-current text-gray-300 group-hover:text-cyan-300" fill-rule="evenodd" d="M2 6a2 2 0 012-2h4l2 2h4a2 2 0 012 2v1H8a3 3 0 00-3 3v1.5a1.5 1.5 0 01-3 0V6z" clip-rule="evenodd" />
                        <path class="fill-current text-gray-600 group-hover:text-cyan-600" d="M6 12a2 2 0 012-2h8a2 2 0 012 2v2a2 2 0 01-2 2H2h2a2 2 0 002-2v-2z" />
                    </svg>
                    <span class="group-hover:text-gray-700">Profile</span>
                </a>
            </li>
            <li>
                <a href="#" class="relative px-4 py-3 flex items-center space-x-4 rounded-xl text-white bg-gradient-to-r from-sky-600 to-cyan-400">
                    <svg xmlns="http://www.w3.org/2000/svg" class="h-5 w-5" viewBox="0 0 20 20" fill="currentColor">
                        <path class="fill-current text-gray-600 group-hover:text-cyan-600" fill-rule="evenodd" d="M2 5a2 2 0 012-2h8a2 2 0 012 2v10a2 2 0 002 2H4a2 2 0 01-2-2V5zm3 1h6v4H5V6zm6 6H5v2h6v-2z" clip-rule="evenodd" />
                        <path class="fill-current text-gray-300 group-hover:text-cyan-300" d="M15 7h1a2 2 0 012 2v5.5a1.5 1.5 0 01-3 0V7z" />
                    </svg>
                    <span class="group-hover:text-gray-700">Control Center</span>
                </a>
            </li>
        </ul>
    </div>

    <div class="px-6 -mx-6 pt-4 flex justify-between items-center border-t">
        <button class="px-4 py-3 flex items-center space-x-4 rounded-md text-gray-600 group">
            <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M17 16l4-4m0 0l-4-4m4 4H7m6 4v1a3 3 0 01-3 3H6a3 3 0 01-3-3V7a3 3 0 013-3h4a3 3 0 013 3v1" />
            </svg>
            <span class="group-hover:text-gray-700">Logout</span>
        </button>
    </div>
</aside>
<div class="ml-auto mb-6 lg:w-[75%] xl:w-[80%] 2xl:w-[85%]">
    <div class="sticky z-10 top-0 h-14 border-b bg-white lg:py-2.5">
        <div class="px-6 flex items-center justify-between space-x-4 2xl:container">
            <h5 hidden class="text-2xl text-gray-600 font-medium lg:block">Control Center</h5>
            <button class="w-12 h-16 -mr-2 border-r lg:hidden">
                <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6 my-auto" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                    <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 6h16M4 12h16M4 18h16" />
                </svg>
        </button>
        </div>
    </div>
 


<!-- component -->
<!-- entire page -->
<main class="flex justify-center pt-12">
  <!-- tabs -->
  <div
    class="grid w-[40rem] grid-cols-3 space-x-2 rounded-xl bg-gray-200 p-2"
    x-data="app"
  >
    <div>
      <input type="radio" name="option" id="1" class="peer hidden" checked />
      <label
        for="1"
        class="block cursor-pointer select-none rounded-xl p-2 text-center peer-checked:bg-blue-500 peer-checked:font-bold peer-checked:text-white peer-checked:bg-gradient-to-r from-sky-600 to-cyan-400"
        >Manual Control</label
      >
    </div>

    <div>
      <input type="radio" name="option" id="2" class="peer hidden" />
      <label
        for="2"
        class="block cursor-pointer select-none rounded-xl p-2 text-center peer-checked:bg-blue-500 peer-checked:font-bold peer-checked:text-white peer-checked:bg-gradient-to-r from-sky-600 to-cyan-400""
        >Schedule Control</label
      >
    </div>

    <div>
      <input type="radio" name="option" id="3" class="peer hidden" />
      <label
        for="3"
        class="block cursor-pointer select-none rounded-xl p-2 text-center peer-checked:bg-blue-500 peer-checked:font-bold peer-checked:text-white peer-checked:bg-gradient-to-r from-sky-600 to-cyan-400"
        >Automatic Control</label
      >
    </div>
  </div>
</main>



<div class="flex space-x-2 justify-center pt-10">
  <button
    type="button"
    data-mdb-ripple="true"
    data-mdb-ripple-color="light"
    class="inline-block px-6 py-2.5 bg-blue-600 text-white font-medium text-xs leading-tight uppercase rounded shadow-md hover:bg-blue-700 hover:shadow-lg focus:bg-blue-700 focus:shadow-lg focus:outline-none focus:ring-0 active:bg-blue-800 active:shadow-lg transition duration-150 ease-in-out"
  >Submit</button>
</div>

</div>
</div>