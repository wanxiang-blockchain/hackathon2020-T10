
import fetch from 'isomorphic-unfetch'

export default (req, res) => {
  const {

    query: { PoEHash },
    method,
    headers,
  } = req

  let result 
  const handleError = err => console.error(err)

  switch(method) {
    case 'POST':
      break      
    case 'GET':
      const url = `${process.env.VERIFY_HOST}action/PoEQuery`
      console.log(' url ', url)
      return fetch(url, {
        method: 'POST', 
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ PoeHash: PoEHash })
      }).then( res => res.json() )
        .then( data => {
          console.log(' PoEQuery reutrn ', data)
          res.json(data)
        } )
      break      
    default:
  }
}

