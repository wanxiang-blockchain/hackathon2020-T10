import React, { useState, useEffect } from 'react'
import { Spin } from 'antd'

const LoadingMask = () => {
  return (
    <>
    <div className="loading-container">
      <Spin size="large" />
    </div>
    <style jsx>{`
      .loading-container {
        position: absolute;
        text-align: center;
        width: 94%;
        height: 94%; 
        /*background: rgba(8,8,8,0.3);*/
      }
    `}</style>
    </>
  )

}

export default LoadingMask;

