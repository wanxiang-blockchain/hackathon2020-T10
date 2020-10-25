import React, { useState, useEffect } from 'react'
import useSWR from 'swr'
import moment from 'moment'
import { Breadcrumb, Table } from 'antd'
import { usePagination, useRowSelection } from '../utils/hooks'
import '../styles/Home.module.less'

const Crumb = () => (
  <Breadcrumb>
    <Breadcrumb.Item>
      <a href='/'>Home </a>
    </Breadcrumb.Item>
    <Breadcrumb.Item> Smart Pen </Breadcrumb.Item>
  </Breadcrumb>
)

const SmartPen = () => {

  const url = `${process.env.APIV1}smartpens?token=${process.env.API_TOKEN}` 
  const { data, error } = useSWR( ['penlist'], () => fetch(url, {
    method: 'GET',
    headers: { 'Content-Type': 'application/json' }
  })
    .then( res => res.json() )
    .then( data => data.map ( ({id,  GpenID, public_key, updated_at }) => ( {
      key: id, 
      id,
      GpenID, 
      public_key,
      updated_at
    }) ))
  )

  const columns = [
    { title: 'id', dataIndex: 'id', key: 'id' },
    { title: '编号', dataIndex: 'GpenID', key: 'GpenID' },
    { title: '公钥', dataIndex: 'public_key', key: 'public_key' },
    { title: '更新时间', key: 'updated_at', dataIndex: 'updated_at',
      render: (text, record) => {
        return record.updated_at && moment(record.updated_at).format('YYYY-MM-DD HH:mm:ss')
      }
    },
  ]

  error && console.error( 'swr return ', error )

  // Table -- selected rows
  const { rowSelection } = useRowSelection()

  // Table -- Page
  const url1 = `${process.env.APIV1}smartpens/count?token=${process.env.API_TOKEN}` 
  const { currentPage, setCurrentPage, pagination } = usePagination('smartpens', url1)
  

  return (
    <main className='main'>
      <div style={{paddingLeft: '50px', width: '100%'}}>
        <Crumb />
      </div>
      <div style={{padding: '20px 50px', width: '100%'}}>
        <Table dataSource={data} columns={columns} 
          rowSelection={rowSelection}
          pagination={pagination}
          bordered={true}/>
      </div>

    </main>
  )
}

export default SmartPen;
