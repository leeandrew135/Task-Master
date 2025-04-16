/**
 * @brief The file which renders screen components based on the route.
 *
 * This file is what actually renders all of the screens based on the
 * specified route. When the route is changed, the BrowserRouter
 * element changes the route and displays the new screen, while 
 * preserving the data in localStorage.

 * @author Andrew Lee
 * @author Ali Farhangi
 */

import { BrowserRouter, Routes, Route, Link } from "react-router-dom";
import React from "react";
import logo from "./logo.svg";
import "./App.css";

// IMPORT ALL PAGE COMPONENTS
import { PageHome, Kanban, PageLogin, Calendar, PageProject } from "./pages";

function App() {
  return (
    <BrowserRouter>
      <div id="mainDiv">
        <Routes>
          <Route path="/" element={<PageLogin />} />
          <Route path="/home" element={<PageHome />} />
          <Route path="/project/:projectId" element={<Kanban />} />
          <Route path="/calendar" element={<Calendar />} />
        </Routes>
      </div>
    </BrowserRouter>
  );
}

export default App;
