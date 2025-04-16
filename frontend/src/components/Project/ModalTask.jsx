import "./styles/StyleModalTask.css";
import { useState } from "react";

const ModalTask = ({ onClick, taskID }) => {
  const backgroundClicked = () => {
    onClick();
  };

  return (
    <div className="modal-background" onClick={backgroundClicked}>
      <div
        className="container-home-modal"
        onClick={(e) => e.stopPropagation()}
      ></div>
    </div>
  );
};

export default ModalTask;
